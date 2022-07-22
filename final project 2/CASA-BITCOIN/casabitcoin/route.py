from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_login.utils import logout_user
from flask_session import Session
from casabitcoin.forms import RegistrationForm, LoginForm, UpdateAccountForm
from flask_login import login_user, current_user, LoginManager, login_required
import sqlite3
from datetime import datetime
from flask_bcrypt import Bcrypt
import requests
import os
from casabitcoin.models import User
import secrets
from PIL import Image
from newsapi import NewsApiClient
import pandas as pd


# configuration
app = Flask(__name__)
app.config['SECRET_KEY'] = "d6ea7c5664afff24c5d065fc659d38a2"
connection = sqlite3.connect("casabitcoin/Database/casadatabase.db")
db = connection.cursor()
login_manager = LoginManager()
login_manager.init_app(app)
login_manager.login_view = 'login'
login_manager.login_message = 'Please log in to access this page.'
bcrypt = Bcrypt(app)
newsapikey = '4a95534541b24b4c9d56d55ae3c4e9c8'
newsapi = NewsApiClient(api_key=newsapikey)


@login_manager.user_loader
def load_user(user_id):
    conn = sqlite3.connect('casabitcoin/Database/casadatabase.db')
    curs = conn.cursor()
    g = curs.execute(
        "SELECT id,email,passwordhash from users where id = (?)", [user_id])
    lu = g.fetchone()
    if lu is None:
        return None
    else:
        return User(int(lu[0]), lu[1], lu[2])


@app.route('/')
def index():
    today = datetime.today().strftime('%Y-%m-%d')
    # news from newsapi
    data = newsapi.get_everything(
        q='bitcoin NOT ethereum NOT dogecoin NOT solana', language='en', page_size=9, from_param=today)
    articles = data['articles']

    # print(articles[0].keys())
    # print(articles[0]['title'])
    # print(articles[0]['url'])
    # dict_keys(['source', 'author', 'title', 'description', 'url', 'urlToImage', 'publishedAt', 'content'])

    return render_template('index.html', articles=articles)


def save_picture(form_picture):
    random_hex = secrets.token_hex(8)
    _, f_ext = os.path.splitext(form_picture.filename)
    picture_fn = random_hex + f_ext
    picture_path = os.path.join(
        app.root_path, 'static/Pictures', picture_fn)

    output_size = (125, 125)
    i = Image.open(form_picture)
    i.thumbnail(output_size)
    i.save(picture_path)

    return picture_fn


@app.route('/account', methods=["POST", "GET"])
@login_required
def account():
    con = sqlite3.connect("casabitcoin/Database/casadatabase.db")

    db = con.cursor()

    form = UpdateAccountForm()
    if form.validate_on_submit():
        if form.picture.data:
            picture_file = save_picture(form.picture.data)
            image = db.execute(("UPDATE users SET picture=? WHERE id = ?"), (picture_file,
                                                                             current_user.id))
            con.commit()
            current_user.image = picture_file

        Email = db.execute(("UPDATE users SET email=? WHERE id=?"),
                           (form.email.data, current_user.id))
        Email = con.commit()
        db.close()
        current_user.email = form.email.data

        flash('Account has been updated', 'success')
        redirect(url_for('account'))
    elif request.method == "GET":
        form.email.data = current_user.email

    image_file = url_for(
        'static', filename='Pictures/' + current_user.image)

    return render_template("account.html", image_file=image_file, form=form)


@app.route('/admin2', methods=["POST", "GET"])
@login_required
def admin():
    # if user not admin -> error
    if current_user.id != 6:
        return render_template("error.html", message='This area is reserved for admin only.')

    # Create a SQL connection to our SQLite database
    con = sqlite3.connect("casabitcoin/Database/casadatabase.db")

    db = con.cursor()

    # get the transactionID from the specific button clicked
    if request.method == "POST":
        transactionID = list(request.form)
        transactionID = transactionID[0]

        # send info in variables
        # get history from user
        history = db.execute(
            "SELECT TransactionID, UsersID, DateTime, coinSymbol, PriceReais, coinQuantity, Total, TypeTrade, Comments, Status FROM History WHERE TransactionID=? ORDER BY TransactionID DESC", [transactionID])
        history = db.fetchall()

        # get size of table
        size = db.execute(
            "SELECT COUNT(TransactionID) FROM History WHERE TransactionID=?", [transactionID])
        size = db.fetchone()

        # update price automatically once number of coins has been updated
        price = history[0][4]
        numbercoins = history[0][5]
        total = history[0][6]
        if numbercoins != 'Pending' and numbercoins != '--':
            price = total / float(numbercoins)
            price = '{0:.2f}'.format(price)

        # close connection
        db.close()

        return render_template('admin_edit_transaction.html', history=history, size=size, price=price)

    # get history from user
    history = db.execute(
        "SELECT TransactionID, UsersID, DateTime, coinSymbol, PriceReais, coinQuantity, Total, TypeTrade, Comments, Status FROM History ORDER BY TransactionID DESC")
    history = db.fetchall()

    # get size of table
    size = db.execute(
        "SELECT COUNT(TransactionID) FROM History")
    size = db.fetchone()

    # close connection
    db.close()

    return render_template('admin.html', history=history, size=size)


@app.route('/edittransaction', methods=["POST", "GET"])
def edittransaction():
    if request.method == "POST":
        # Create a SQL connection to our SQLite database
        con = sqlite3.connect("casabitcoin/Database/casadatabase.db")
        db = con.cursor()

        # set variables
        transaction_id = request.form.get("transaction_id")
        user_id = request.form.get("user_id")
        date_time = request.form.get("date_time")
        coin_symbol = request.form.get("coin_symbol")
        price = request.form.get("price")
        number_coins = request.form.get("number_coins")
        total = request.form.get("total")
        transaction_type = request.form.get("transaction_type")
        comments = request.form.get("comments")
        status = request.form.get("status")

        db.execute(("UPDATE history SET TransactionID=?, UsersID=?, DateTime=?, coinSymbol=?, PriceReais=?, coinQuantity=?, Total=?, TypeTrade=?, Status=?, Comments=? WHERE TransactionID=?"),
                   (transaction_id, user_id, date_time, coin_symbol, price, number_coins, total, transaction_type, status, comments, transaction_id))

        con.commit()

        # If status confirmed, update balances
        if status == 'Confirmed':

            # balance of reais
            reaisbalance = db.execute(
                "SELECT reaisbal FROM users WHERE id=?", [user_id])
            reaisbalance = db.fetchone()

            # balance of dollars
            dollarbalance = db.execute(
                "SELECT dollarbal FROM users WHERE id=?", [user_id])
            dollarbalance = db.fetchone()

            # balance of BTC
            bitcoinbalance = db.execute(
                "SELECT btcbal FROM users WHERE id=?", [user_id])
            bitcoinbalance = db.fetchone()

            # if buy, new balances
            # (at the moment, since all buys are 'deposit and buy', reais balance are unchanged)
            if transaction_type == 'Buy':
                newreaisbal = reaisbalance[0]

                # new balance of BTC
                if coin_symbol == "Bitcoin":
                    newbtcbalance = bitcoinbalance[0] + float(number_coins)

                    # update balance
                    db.execute(("UPDATE users SET reaisbal=?, btcbal=? WHERE id=?"),
                               (newreaisbal, newbtcbalance, user_id))

                if coin_symbol == "Dollar":
                    newdollarbalance = dollarbalance[0] + float(number_coins)

                    # update balance
                    db.execute(("UPDATE users SET reaisbal=?, dollarbal=? WHERE id=?"),
                               (newreaisbal, newdollarbalance, user_id))

            # if sell, new balances
            if transaction_type == 'Sell':
                newreaisbal = reaisbalance[0]

                if coin_symbol == "Bitcoin":
                    # insufficient coins
                    if float(number_coins) > bitcoinbalance[0]:
                        db.execute(("UPDATE history SET Status=? WHERE TransactionID=?"),
                                   ('Insufficient Coins', transaction_id))

                        con.commit()

                        # close connection
                        db.close()

                        flash(
                            'User has insufficient coins for this sell order', 'danger')

                        return redirect('/admin2')

                    newbtcbalance = bitcoinbalance[0] - float(number_coins)

                    newreaisbal = reaisbalance[0] + float(total)

                    # update balance
                    db.execute(("UPDATE users SET reaisbal=?, btcbal=? WHERE id=?"),
                               (newreaisbal, newbtcbalance, user_id))

                if coin_symbol == "Dollar":
                    # insufficient coins
                    if float(number_coins) > dollarbalance[0]:
                        db.execute(("UPDATE history SET Status=? WHERE TransactionID=?"),
                                   ('Insufficient Coins', transaction_id))

                        con.commit()

                        # close connection
                        db.close()

                        flash(
                            'User has insufficient coins for this sell order', 'danger')

                        return redirect('/admin2')
                    newdollarbalance = dollarbalance[0] - float(number_coins)

                    newreaisbal = reaisbalance[0] + float(total)

                    # update balance
                    db.execute(("UPDATE users SET reaisbal=?, dollarbal=? WHERE id=?"),
                               (newreaisbal, newdollarbalance, user_id))

            # if withdrawal, new balances
            if transaction_type == 'Withdrawal':
                if coin_symbol == 'Real (withdrawal)':
                    newreaisbal = reaisbalance[0] - float(total)

                    # update balance
                    db.execute(("UPDATE users SET reaisbal=? WHERE id=?"),
                               (newreaisbal, user_id))
                else:
                    db.execute(("UPDATE history SET Status=? WHERE TransactionID=?"),
                               ('Pending', transaction_id))

                    con.commit()

                    # close connection
                    db.close()

                    flash(
                        'You can only withdraw with Reais. This transaction will be Pending until you change Coin Symbol.', 'danger')

                    return redirect('/admin2')

            # update price if number of coins filled
            if number_coins != 0 and number_coins != '--':
                price = float(total) / float(number_coins)
                price = '{0:.2f}'.format(price)

                db.execute(
                    ("UPDATE history SET PriceReais=? WHERE TransactionID=?"), (price, transaction_id))

            # commit changes
            con.commit()

            flash('Transaction Confirmed!', 'success')

        # close connection
        db.close()

        flash('Transaction Updated!', 'success')

    return redirect('/admin2')


@app.route('/login', methods=["POST", "GET"])
def login():
    con = sqlite3.connect("casabitcoin/Database/casadatabase.db")

    db = con.cursor()

    if current_user.is_authenticated:
        return render_template('error.html')

    form = LoginForm()

    if form.validate_on_submit():
        user_id = db.execute(
            "SELECT id from users WHERE email = ?", [form.email.data])

        user_id = list(db.fetchone())

        user_object = load_user(user_id[0])

        email = form.email.data

        if email and bcrypt.check_password_hash(user_object.password, form.password.data):
            login_user(user_object, remember=form.remember.data)
            # close connection
            db.close
            flash(f'Welcome back {form.email.data}', 'success')
            return redirect(url_for('index'))
        else:
            # close connection
            db.close()
            flash('Login unsuccessful. Please check password and/or email!', 'danger')
            return render_template("login.html")

    return render_template('login.html', form=form)


@app.route('/history')
@login_required
def history():

    # Create a SQL connection to our SQLite database
    con = sqlite3.connect("casabitcoin/Database/casadatabase.db")

    db = con.cursor()

    # get history from user
    history = db.execute(
        "SELECT TransactionID, DateTime, coinSymbol, PriceReais, coinQuantity, Total, TypeTrade, Comments, Status FROM History WHERE UsersID=? ORDER BY TransactionID DESC", [current_user.id])
    history = db.fetchall()

    # get size of table
    size = db.execute(
        "SELECT COUNT(TransactionID) FROM History WHERE UsersID=?", [current_user.id])
    size = db.fetchone()

    # close connection
    db.close()

    return render_template('history.html', history=history, size=size)


@app.route('/holdings', methods=["GET", "POST"])
@login_required
def holdings():

    # Create a SQL connection to our SQLite database
    con = sqlite3.connect("casabitcoin/Database/casadatabase.db")

    db = con.cursor()

    # balance of reais
    reaisbalance = db.execute(
        "SELECT reaisbal FROM users WHERE id=?", [current_user.id])
    reaisbalance = db.fetchone()

    # balance of dollars
    dollarbalance = db.execute(
        "SELECT dollarbal FROM users WHERE id=?", [current_user.id])
    dollarbalance = db.fetchone()

    # balance of BTC
    bitcoinbalance = db.execute(
        "SELECT btcbal FROM users WHERE id=?", [current_user.id])
    bitcoinbalance = db.fetchone()

    # numbers of decimals
    bitcoinbalance = '{0:.8f}'.format(bitcoinbalance[0])
    dollarbalance = '{0:.2f}'.format(dollarbalance[0])
    reaisbalance = '{0:.2f}'.format(reaisbalance[0])

    # close connection
    db.close()

    return render_template('holdings.html', reaisbalance=reaisbalance, dollarbalance=dollarbalance, bitcoinbalance=bitcoinbalance)


@app.route('/market', methods=["POST", "GET"])
@login_required
def market():
    if request.method == "POST":

        # Create a SQL connection to our SQLite database
        con = sqlite3.connect("casabitcoin/Database/casadatabase.db")
        db = con.cursor()

        # current date/time
        currentDateTime = datetime.now()

        # dd/mm/YY H:M:S
        dt_string = currentDateTime.strftime("%d/%m/%Y %H:%M:%S")

        # determine if submit button clicked was buy or sell
        if request.form["button"] == 'buy':
            # set variables
            coinBuy = request.form.get("coinBuy")
            totalBuy = request.form.get("totalBuy")
            commentBuy = request.form.get("commentBuy")

            # insert into history db
            db.execute(("INSERT INTO History (UsersID, DateTime, coinSymbol, PriceReais, coinQuantity, Total, TypeTrade, Status, Comments) VALUES (?, ?, ?, 'Pending', 'Pending', ?, 'Buy', 'Pending', ?)"),
                       (current_user.id, dt_string, coinBuy, totalBuy, commentBuy))

            con.commit()

            # get history from user
            history = db.execute(
                "SELECT TransactionID, DateTime, coinSymbol, PriceReais, coinQuantity, Total, TypeTrade, Comments, Status FROM History WHERE UsersID=? ORDER BY TransactionID DESC", [current_user.id])
            history = db.fetchall()

            # get size of table
            size = db.execute(
                "SELECT COUNT(TransactionID) FROM History WHERE UsersID=?", [current_user.id])
            size = db.fetchone()

            # commit
            con.commit()

            # message flash
            flash("Your order has been received. Within the next 24 hours your order will be confirmed.", 'success')

            # close connection
            db.close()

            return render_template("history.html", history=history, size=size)

        elif request.form["button"] == 'sell':
            # set variables
            coinSell = request.form.get("coinSell")
            totalSell = request.form.get("totalSell")
            commentSell = request.form.get("commentSell")

            # insert into history db
            db.execute(("INSERT INTO History (UsersID, DateTime, PriceReais, coinSymbol, coinQuantity, Total, TypeTrade, Status, Comments) VALUES (?, ?, 'Pending', ?, 'Pending', ?, 'Sell', 'Pending', ?)"),
                       (current_user.id, dt_string, coinSell, totalSell, commentSell))

            # if no bank details in db, get them from user
            checkdetails = db.execute(
                "SELECT COUNT(cpf) FROM bankdetails WHERE usersID=?", [current_user.id])
            checkdetails = db.fetchone()
            if checkdetails[0] == 0:

                # status missing bank details
                db.execute((
                    "UPDATE History SET Status='Missing Details' WHERE usersID=? AND TransactionID=(SELECT TransactionID FROM History WHERE usersID=? ORDER BY TransactionID DESC LIMIT 1)"), (current_user.id, current_user.id))

                # commit
                con.commit()

                # close connection
                db.close()

                return render_template("bankdetails.html")

            # get history from user
            history = db.execute(
                "SELECT TransactionID, DateTime, coinSymbol, PriceReais, coinQuantity, Total, TypeTrade, Comments, Status FROM History WHERE UsersID=? ORDER BY TransactionID DESC", [current_user.id])
            history = db.fetchall()

            # get size of table
            size = db.execute(
                "SELECT COUNT(TransactionID) FROM History WHERE UsersID=?", [current_user.id])
            size = db.fetchone()

            # message flash
            flash("Your order has been received. Within the next 24 hours your order will be confirmed.", 'success')

            # commit
            con.commit()

            # close connection
            db.close()

            return render_template("history.html", history=history, size=size)

        elif request.form["button"] == 'withdraw':
            withdraw_amount = request.form.get("withdraw")

            # check balance of reais
            reaisbalance = db.execute(
                "SELECT reaisbal FROM users WHERE id=?", [current_user.id])
            reaisbalance = db.fetchone()

            if float(withdraw_amount) > reaisbalance[0]:
                # message flash
                flash("Insufficient funds (Reais) to withdraw", 'danger')

                # close connection
                db.close()

                return redirect('/market')

            # insert into history db
            db.execute(("INSERT INTO History (UsersID, DateTime, coinSymbol, PriceReais, coinQuantity, Total, TypeTrade, Status) VALUES (?, ?, 'Real (withdrawal)', '--', '--', ?, 'Withdrawal', 'Pending')"),
                       (current_user.id, dt_string, withdraw_amount, ))

            con.commit()

            # if no bank details in db, get them from user
            checkdetails = db.execute(
                "SELECT COUNT(cpf) FROM bankdetails WHERE usersID=?", [current_user.id])
            checkdetails = db.fetchone()

            if checkdetails[0] == 0:

                # status missing bank details
                db.execute((
                    "UPDATE History SET Status='Missing Details' WHERE usersID=? AND TransactionID=(SELECT TransactionID FROM History WHERE usersID=? ORDER BY TransactionID DESC LIMIT 1)"), (current_user.id, current_user.id))

                # commit
                con.commit()

                # close connection
                db.close()

                return render_template("bankdetails.html")

            # get history from user
            history = db.execute(
                "SELECT TransactionID, DateTime, coinSymbol, PriceReais, coinQuantity, Total, TypeTrade, Comments, Status FROM History WHERE UsersID=? ORDER BY TransactionID DESC", [current_user.id])
            history = db.fetchall()

            # get size of table
            size = db.execute(
                "SELECT COUNT(TransactionID) FROM History WHERE UsersID=?", [current_user.id])
            size = db.fetchone()

            # commit
            con.commit()

            # message flash
            flash("Your withdrawal order has been received. Within the next 24 hours your order will be confirmed.", 'success')

            # close connection
            db.close()

            return render_template("history.html", history=history, size=size)

        # close connection
        db.close()

    return render_template('market.html')


@app.route('/confirmed', methods=["POST", "GET"])
@login_required
def confirmed():
    if request.method == "POST":
        # Create a SQL connection to our SQLite database
        con = sqlite3.connect("casabitcoin/Database/casadatabase.db")
        db = con.cursor()

        # setup variables
        bank = request.form.get("bank")
        sortcode = request.form.get("sortcode")
        account = request.form.get("account")
        pix = request.form.get("pix")
        cpf = request.form.get("cpf")

        # check existence of records just in case
        checkdetails = db.execute(
            "SELECT COUNT(cpf) FROM bankdetails WHERE usersID=?", [current_user.id])
        checkdetails = db.fetchall()
        if checkdetails[0][0] != 0:
            return render_template("error.html", message='We already have your bank details')

        # insert bank details into db
        db.execute(("INSERT INTO bankdetails (usersID, bank, sortcode, account, pix, cpf) VALUES (?, ?, ?, ?, ?, ?)"),
                   (current_user.id, bank, sortcode, account, pix, cpf))

        # update status
        db.execute(("UPDATE History SET Status='Pending' WHERE Status='Missing Details' AND usersID=? AND TransactionID=(SELECT TransactionID FROM History WHERE usersID=? ORDER BY TransactionID DESC LIMIT 1)"), (current_user.id, current_user.id))

        # get history from user
        history = db.execute(
            "SELECT TransactionID, DateTime, coinSymbol, PriceReais, coinQuantity, Total, TypeTrade, Comments, Status FROM History WHERE UsersID=? ORDER BY TransactionID DESC", [current_user.id])
        history = db.fetchall()

        # get size of table
        size = db.execute(
            "SELECT COUNT(TransactionID) FROM History WHERE UsersID=?", [current_user.id])
        size = db.fetchone()

        # commit
        con.commit()

        # message flash
        flash("Your order has been received. Within the next 24 hours your order will be confirmed.", 'success')

        # close connection
        db.close()

        return render_template("history.html", history=history, size=size)


@app.route('/bankdetails', methods=["POST", "GET"])
@login_required
def bankdetails():
    return render_template("bankdetails.html")


@app.route('/register', methods=["POST", "GET"])
def register():

    con = sqlite3.connect("casabitcoin/Database/casadatabase.db")
    db = con.cursor()

    if current_user.is_authenticated:
        return redirect(url_for('index.html'))

    form = RegistrationForm()

    if form.validate_on_submit():
        hashed_passwword = bcrypt.generate_password_hash(
            form.password.data).decode('utf-8')

        db.execute("INSERT INTO users(email ,fullname , cpf , rg , address , dateofbirth , ppe ,passwordhash ,frontpic ,backpic ,selfie)  VALUES (? ,? ,? ,? ,? ,? ,? ,? ,? ,? ,?)",
                   (form.email.data, form.fullName.data, form.cpf.data, form.rg.data, form.fullAddress.data, form.dateOfBirth.data, form.ppe.data, hashed_passwword, 'soon', 'soon', 'soon'))
        con.commit()
        flash(f'Account created for {form.fullName.data}!', 'success')
        return redirect(url_for('login'))
    # close connection
    db.close()
    return render_template("register.html", form=form)


@app.route('/contactus')
def contactus():
    return render_template("contactus.html")


@app.route('/logout')
@login_required
def logout():
    logout_user()
    return redirect(url_for('index'))
