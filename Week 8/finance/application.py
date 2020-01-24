import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session, url_for
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime
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
    userid = session["user_id"]
    rows = db.execute("SELECT * from users join tracks on users.id = tracks.userid WHERE tracks.userid=:id", id=userid)        
    return render_template("main.htm", rows=rows, length=len(rows))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol").upper()
        shares = int(request.form.get("shares"))
        number_of_shares = int(shares)
        result = lookup(symbol)

        if not symbol or not number_of_shares:
            flash("You must input both Symbol and Shares!", 'danger')
            return redirect('/buy')

        elif number_of_shares <= 0:
            flash("Number of shares must be more than 0!", 'danger')
            return redirect('/buy')

        elif result is None:
            flash(f"'{symbol}' doesn't exist!", 'danger')
            return redirect('/buy')

        else:
            cash = db.execute("SELECT cash FROM users WHERE username = :username", username=session["user_name"])
            cash = cash[0]["cash"]
            name = result["name"]
            price = result["price"]
            username = session["user_name"]
            userid = session["user_id"]
            now = datetime.now()
            current_time = now.strftime("%Y-%m-%d %H:%M:%S")
            check = db.execute("SELECT * from tracks where symbol =:sym and userid=:userid",
             sym=symbol, userid=userid)
            if len(check) == 0:
                db.execute("INSERT INTO tracks (userid, shares, name, symbol, price, date) values(:userid, :shares, :name, :symbol, :price, :d)",
                userid = userid, shares=shares, name=name, symbol=symbol, price=price, d=current_time)
                db.execute("UPDATE users set cash=:new_cash where id=:userid;",
                new_cash = cash - (price * shares), userid=session["user_id"])
            
            else:
                db.execute("UPDATE tracks set shares = :new_shares where symbol=:sym;",
                new_shares = check[0]["shares"] + shares, sym=symbol )
                db.execute("UPDATE users set cash=:new_cash where id=:userid;",
                new_cash = cash - (price * shares), userid=session["user_id"])
            db.execute("INSERT INTO history(userid, symbol, shares, price, date) values(:userid, :symbol, :shares, :price, :d)",
            userid=userid, symbol=symbol, shares=shares, price=price * shares, d=current_time)
            flash(f"{number_of_shares} shares of {symbol.upper()} were bought!", 'success')
            return redirect('/')
    else:
        return render_template("buy.htm")


@app.route("/history")
@login_required
def history():
    rows = db.execute("SELECT * FROM history where userid=:id;", id=session["user_id"])
    return render_template("history.htm", rows=rows, length=len(rows))


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        session.clear()
        # Ensure username was submitted
        if not request.form.get("username"):
            flash("must provide username", 'danger')
            return redirect('/login')

        # Ensure password was submitted
        elif not request.form.get("password"):
            flash("must provide password", 'danger')
            return redirect('/login')

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            flash("invalid username and/or password", 'danger')
            return redirect('/login')

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]
        session["user_name"] = rows[0]["username"]
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
    if request.method == "POST":
        stock_quote = request.form.get("quote").upper()
        result = lookup(stock_quote)
        if result is None:
            msg = "Stock symbol not exists!"
        else:
            name = result["name"]
            price = result["price"]
            symbol = result["symbol"]
            msg = f"A share of {name} ({symbol}) costs ${price}"
        return render_template("quoted.htm", msg=msg)
    else:
        return render_template("quote.htm")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        name = request.form.get("username")
        password = request.form.get("password")
        repeated_password = request.form.get("rpassword")
        rows = db.execute("SELECT * FROM users where username=:username", username=name.lower())
        if len(rows) > 0:
            flash(f"Username '{name}' already exists!", 'danger')
        elif password != repeated_password:
            flash("Passwords don't match!", 'danger')
        else:
            db.execute("INSERT INTO users(username, hash) values(:name, :password)",
             name=name, password=generate_password_hash(password))
            flash("Account created successfully", 'success')
        return redirect("/register")
    else:
        return render_template("register.htm")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        return redirect("/")
    else:
        return render_template("sell.htm")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
