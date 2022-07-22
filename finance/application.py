import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
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

# create History table
db.execute("CREATE TABLE IF NOT EXISTS History (TransactionID INTEGER PRIMARY KEY, UsersID int NOT NULL, DateTime text NOT NULL, Symbol text NOT NULL, SymbolBalance int NOT NULL, Price int NOT NULL, Shares int NOT NULL, Total int NOT NULL, Type text NOT NULL, FOREIGN KEY (UsersID) REFERENCES users(id))")


# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/", methods=["GET", "POST"])
@login_required
def index():
    """Show portfolio of stocks"""

    deposit = request.form.get('deposit')

    if request.method == 'POST':

        currentBalance = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])

        newBalance = float(currentBalance[0]['cash']) + float(deposit)

        db.execute("UPDATE users SET cash=? WHERE id=?", newBalance, session["user_id"])

        return redirect("/")

    portfolio = db.execute("SELECT * FROM History WHERE UsersID=?", session["user_id"])

    # updated number of shares for each stock
    nshares = db.execute("SELECT SymbolBalance, MAX(DateTime) FROM History WHERE UsersID=? GROUP BY Symbol", session["user_id"])

    # get unique stocks owned
    stocks = db.execute("SELECT Symbol FROM (SELECT Symbol, SymbolBalance, MAX(DateTime) FROM History WHERE UsersID=? GROUP BY Symbol) WHERE SymbolBalance!=0",
                        session["user_id"])

    # number of rows
    stockVariety = len(stocks)

    # current price of each stock
    currentprice = []
    for i in range(stockVariety):
        currentprice.append(lookup(stocks[i]['Symbol']))

    # total holding
    totalH = []
    for i in range(stockVariety):
        totalH.append(currentprice[i]['price'] * nshares[i]['SymbolBalance'])

    # stock total
    stockTotal = 0
    for i in range(stockVariety):
        stockTotal += totalH[i]

    # cash total
    currentBalance = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])

    # total balance
    totalBalance = stockTotal + currentBalance[0]['cash']

    return render_template("index.html", portfolio=portfolio, stocks=stocks, stockVariety=stockVariety, nshares=nshares, currentprice=currentprice, totalH=totalH, currentBalance=currentBalance, stockTotal=stockTotal, totalBalance=totalBalance, deposit=deposit)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        # Ensure symbol and shares was submitted
        symbol = request.form.get("symbol")
        sharesNumber = request.form.get("shares")

        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        if not request.form.get("shares"):
            return apology("must provide number of shares", 400)

        if not sharesNumber.isdigit():
            return apology("number of shares invalid.", 400)

        # lookup stock
        quote = {}
        quote = lookup(symbol)

        if quote == None:
            return apology("must provide a valid symbol", 400)

        # check if user has the funds to purchase
        currentBalance = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])

        totalBuy = quote['price'] * float(sharesNumber)

        if totalBuy > float(currentBalance[0]['cash']):
            return apology("Your current balance is lower than your intended purchase", 403)

        # current date/time
        currentDateTime = datetime.now()

        # dd/mm/YY H:M:S
        dt_string = currentDateTime.strftime("%d/%m/%Y %H:%M:%S")

        # update current balance of stock
        temp = db.execute("SELECT SymbolBalance FROM History WHERE UsersID=? AND Symbol=? ORDER BY DateTime DESC LIMIT 1",
                          session["user_id"], quote['symbol'])

        zero = db.execute("SELECT COUNT(TransactionID) FROM History WHERE UsersID=? AND Symbol=? ORDER BY DateTime DESC LIMIT 1",
                          session["user_id"], quote['symbol'])

        if zero[0]['COUNT(TransactionID)'] == 0:
            StockBalance = int(sharesNumber)
        else:
            StockBalance = temp[0]['SymbolBalance'] + int(sharesNumber)

        # insert into table information
        db.execute("INSERT INTO History (UsersID, DateTime, Symbol, SymbolBalance, Price, Shares, Total, Type) VALUES (?, ?, ?, ?, ?, ?, ?, 'Buy')",
                   session["user_id"], dt_string, quote['symbol'], StockBalance, quote['price'], sharesNumber, totalBuy)

        # update user's balance
        newBalance = float(currentBalance[0]['cash']) - totalBuy

        db.execute("UPDATE users SET cash=? WHERE id=?", newBalance, session["user_id"])

        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # get history from user
    history = db.execute(
        "SELECT TransactionID, DateTime, Symbol, Price, Shares, Total, Type FROM History WHERE UsersID=?", session["user_id"])
    size = db.execute("SELECT COUNT(TransactionID) FROM History WHERE UsersID=?", session["user_id"])

    return render_template("history.html", history=history, size=size)


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        print(session["user_id"])

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

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        # lookup quote and render page with quote (quoted)
        symbol = request.form.get("symbol")
        quote = {}
        quote = lookup(symbol)

        if (quote == None):
            return apology("stock symbol doesn't exist", 400)

        return render_template("quoted.html", symbol=quote['symbol'], price=quote['price'])

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure passwords match + apology (if password and confirmation don't match)
        pass1 = request.form.get("password")
        pass2 = request.form.get("confirmation")

        if pass1 != pass2:
            return apology("passwords don't match", 400)

        # hash the password
        hash1 = generate_password_hash(pass1, method='pbkdf2:sha256', salt_length=8)

        # username and opology if username already in database
        username = request.form.get("username")

        check = db.execute("SELECT COUNT(id) FROM users WHERE username=?", username)

        if check[0]['COUNT(id)'] != 0:
            return apology("this username is not available", 400)

        # Insert user to database
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hash1)

        return render_template('success.html')

    else:
        return render_template('register.html')


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # get unique stocks owned
    stocks = db.execute("SELECT Symbol FROM History WHERE UsersID=? GROUP BY Symbol", session["user_id"])

    # number of rows
    stockVariety = len(stocks)

    if request.method == "POST":

        # create variables
        shares = request.form.get("shares")
        symbol = request.form.get("symbol")

        # Ensure symbol and shares was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        if not request.form.get("shares"):
            return apology("must provide number of shares", 400)

        # lookup stock
        quote = {}
        quote = lookup(symbol)

        # total sell
        totalSell = quote['price'] * float(shares)

        # check if user has the stocks to sell
        stockCheck = db.execute("SELECT SUM (Shares) FROM History WHERE UsersID=? GROUP BY ?", session["user_id"], symbol)

        if int(stockCheck[0]['SUM (Shares)']) < int(shares):
            return apology("You do not have enough shares to sell", 400)

        # current date/time
        currentDateTime = datetime.now()

        # dd/mm/YY H:M:S
        dt_string = currentDateTime.strftime("%d/%m/%Y %H:%M:%S")

        # update current balance of stock
        temp = db.execute("SELECT SymbolBalance FROM History WHERE UsersID=? AND Symbol=? ORDER BY DateTime DESC LIMIT 1",
                          session["user_id"], quote['symbol'])

        zero = db.execute("SELECT COUNT(TransactionID) FROM History WHERE UsersID=? AND Symbol=? ORDER BY DateTime DESC LIMIT 1",
                          session["user_id"], quote['symbol'])

        if zero[0]['COUNT(TransactionID)'] == 0:
            return apology("you do not own part or all of these shares to sell", 400)
        else:
            StockBalance = temp[0]['SymbolBalance'] - int(shares)

        # insert into table information
        db.execute("INSERT INTO History (UsersID, DateTime, Symbol, SymbolBalance, Price, Shares, Total, Type) VALUES (?, ?, ?, ?, ?, ?, ?, 'Sell')",
                   session["user_id"], dt_string, quote['symbol'], StockBalance, quote['price'], int(shares), totalSell)

        # cash total
        currentBalance = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])

        # update user's balance
        newBalance = float(currentBalance[0]['cash']) + totalSell

        db.execute("UPDATE users SET cash=? WHERE id=?", newBalance, session["user_id"])

        return redirect("/")

    else:
        return render_template("sell.html", stocks=stocks, stockVariety=stockVariety)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
