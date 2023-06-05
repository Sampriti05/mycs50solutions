import os
import datetime

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session["user_id"]

    stocks = db.execute("SELECT symbol, name, price, SUM(shares) as totalShares FROM transactions WHERE user_id = ?", user_id)
    cash = db.execute("SELECT cash FROM user WHERE id =?", user_id)[0]["cash"]

    total = cash

    for stock in stocks:
        price = stock["price"]
        totalShares = stock["totalShares"]

        if price is not None and totalShares is not None:
            total += stock["price"] * stock["totalShares"]

    return render_template("index.html", stocks=stocks, cash=cash, usd=usd, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # if request method is GET, display buy.html form
    if request.method == "GET":
        return render_template("buy.html")

    # if request method is POST
    elif request.method == "POST":
        # save stock symbol, number of shares, and quote dict from form
        symbol = request.form.get("symbol").upper()
        shares = request.form.get("shares")
        item = lookup(symbol)

        # return apology if symbol not provided or invalid
        if symbol == None:
            return apology("must provide stock symbol")
        elif not item:
            return apology("must provide valide stock symbol")

        # return apology if shares not provided. buy form only accepts positive integers
        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("Shares must be an integer")

        if shares <= 0:
            return apology("Shares must be a positive integer")

        user_id = session["user_id"]
        cash = db.execute("SELECT cash FROM user WHERE id = ?", user_id)[0]["cash"]

        item_name = item["name"]
        item_price = item["price"]
        total_price = item_price * shares

        if cash < total_price:
            return apology("Not enough cash!")
        else:
            db.execute("UPDATE user SET cash = ? WHERE id = ?", cash - total_price, user_id)
            db.execute("INSERT INTO transactions (user_id, name, shares, price, type, symbol) VALUES (?, ?, ?, ?, ?, ?)",
                       user_id, item_name, shares, item_price, 'buy', symbol)

        return redirect('/')


@app.route("/password", methods=["GET", "POST"])
@login_required
def password():
    """ Change Password """

    # if method GET, display password change form
    if request.method == "GET":
        return render_template("password.html")

    # if method POST, change password
    else:
        # return apologies if form not filled out
        if not request.form.get("oldpass") or not request.form.get("newpass") or not request.form.get("confirm"):
            return apology("missing old or new password", 403)

        # save variables from form
        oldpass = request.form.get("oldpass")
        newpass = request.form.get("newpass")
        confirm = request.form.get("confirm")

        # user's previous password
        hash = db.execute("SELECT hash FROM users WHERE id = :id", id=session["user_id"])
        hash = hash[0]['hash']

        # if old password incorrect, return apology
        if not check_password_hash(hash, oldpass):
            return apology("old password incorrect", 403)

        # if new passwords don't match, return apology
        if newpass != confirm:
            return apology("new passwords do not match", 403)

        # hash new password
        hash = generate_password_hash(confirm)

        # insert new hashed password into users table
        db.execute("UPDATE users SET hash = :hash WHERE id = :id", hash=hash, id=session["user_id"])

        return redirect("/logout")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    transactions = db.execute("SELECT type, symbol, price, shares, time FROM transactions WHERE user_id = ?", user_id)

    return render_template("history.html", transactions=transactions, usd=usd)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM user WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # if GET method, return quote.html form
    if request.method == "GET":
        return render_template("quote.html")

    # if POST method, get info from form, make sure it's a valid stock
    else:

        # lookup ticker symbol from quote.html form
        item = lookup(request.form.get("symbol"))

        # if lookup() returns None, it's not a valid stock symbol
        if item == None:
            return apology("invalid stock symbol")

        # Return template with stock quote, passing in symbol dict
        return render_template("quoted.html", item=item, usd_function=usd)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (submitting the register form)
    if request.method == "POST":
        username = request.form.get('username')
        password = request.form.get('password')
        confirmation = request.form.get('confirmation')

        # ensure username was submitted
        if not username:
            return apology("Username required")

        # ensure password was submitted
        elif not password:
            return apology("Password required")

        # ensure confirm password submitter
        elif not confirmation:
            return apology("Confirmation required")

        # ensure passwords match
        elif password != confirmation:
            return apology("Passwords do not match")

        hash = generate_password_hash(password)

        try:
            db.execute("INSERT INTO user (username, hash) VALUES (?, ?)", username, hash)
            return redirect('/')
        except:
            return apology('Username has been registered')
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":
        user_id = session["user_id"]
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))

        if shares <= 0:
            return apology("Shares must be a positive number")
        item_price = lookup(symbol)['price']
        item_name = lookup(symbol)['name']
        price = shares * item_price

        shares_owned = db.execute("SELECT shares FROM transactions WHERE user_id = ? AND symbol = ? GROUP BY symbol", user_id, symbol)[
            0]["shares"]

        if shares_owned < shares:
            return apology("You dont have enough shares")

        current_cash = db.execute("SELECT cash FROM user WHERE id = ?", user_id)[0]["cash"]
        db.execute("UPDATE user SET cash = ? WHERE id = ?", current_cash + price, user_id)
        db.execute("INSERT INTO transactions (user_id, name, shares, price, type, symbol) VALUES(?, ?, ?, ?, ?, ?)",
                   user_id, item_name, -shares, item_price, "sell", symbol)

        return redirect('/')

    else:
        user_id = session["user_id"]
        symbols = db.execute("SELECT symbol FROM transactions WHERE user_id = ? GROUP BY symbol", user_id)
        return render_template("sell.html", symbols=symbols)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)