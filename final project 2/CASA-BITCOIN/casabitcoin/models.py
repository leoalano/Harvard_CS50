from flask_login import UserMixin
import sqlite3


class User(UserMixin):
    def __init__(self, id, email, password):
        conn = sqlite3.connect('casabitcoin/Database/casadatabase.db')
        db = conn.cursor()
        self.id = id
        self.email = email
        self.password = password
        image = db.execute("SELECT picture FROM users WHERE id = ?",
                           [id])
        image = db.fetchone()
        picture = image[0]
        self.image = picture
        self.authenticated = False
        db.close()

    def is_active(self):
        return self.is_active()

    def is_anonymous(self):
        return False

    def is_authenticated(self):
        return self.authenticated

    def is_active(self):
        return True

    def get_id(self):
        return self.id
