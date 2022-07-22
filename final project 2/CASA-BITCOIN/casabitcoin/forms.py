from flask_wtf import FlaskForm
from flask_wtf.file import FileField, FileAllowed
import sqlite3
from wtforms import StringField, PasswordField, SubmitField, BooleanField, TextAreaField, DateField, SelectField
from wtforms import validators
from wtforms.validators import DataRequired, Length, Email, EqualTo, ValidationError
from flask_login import current_user


class RegistrationForm(FlaskForm):
    fullName = StringField('Full Name',
                           validators=[DataRequired(), Length(min=2, max=20)])

    email = StringField('Email',
                        validators=[DataRequired(), Email()])

    cpf = StringField('CPF',
                      validators=[DataRequired(), Length(min=11, max=11)])

    rg = StringField('RG',
                     validators=[DataRequired(), Length(min=1, max=13)])

    fullAddress = TextAreaField('Full Address',
                                validators=[DataRequired()])

    dateOfBirth = DateField('Date Of Birth',
                            validators=[DataRequired()],)

    ppe = SelectField('Are you a politically exposed person?',
                      choices=[('yes', 'yes'), ('no', 'no')])

    password = PasswordField('Password',
                             validators=[DataRequired(), Length(min=8, max=25)])

    confirm_password = PasswordField('Confirm Password',
                                     validators=[DataRequired(), EqualTo('password', 'password does not match')])

    submit = SubmitField('Sign Up')

    def validate_email(self, email):
        con = sqlite3.connect("casabitcoin/Database/casadatabase.db")

        db = con.cursor()

        Email = db.execute(
            "SELECT email FROM users WHERE email = (?)", [email.data])

        Email = db.fetchone()

        # close connection
        db.close()

        if Email:
            raise ValidationError(
                "That email is taken. Please choose a different one!")


class LoginForm(FlaskForm):
    email = StringField('Email',
                        validators=[DataRequired(), Email()])

    password = PasswordField('Password',
                             validators=[DataRequired(), Length(min=8, max=25)])

    remember = BooleanField('Remember Me')

    submit = SubmitField('Login')

    def validate_email(self, email):
        conn = sqlite3.connect('casabitcoin/Database/casadatabase.db')
        db = conn.cursor()
        Email = db.execute(
            "SELECT email FROM users where email = (?)", [email.data])
        Email = db.fetchone()
        if Email is None:
            raise ValidationError(
                'This Email ID is not registered. Please register before login')


class UpdateAccountForm(FlaskForm):
    email = StringField('Email',
                        validators=[DataRequired(), Email()])

    picture = FileField('Update Profile Picture',
                        validators=[FileAllowed(['jpg', 'png'])])

    submit = SubmitField('Update')

    def validate_email(self, email):
        print(current_user.email)
        if email.data != current_user.email:

            conn = sqlite3.connect('casabitcoin/Database/casadatabase.db')
            db = conn.cursor()
            Email = db.execute(
                "SELECT email FROM users where email = (?)", [email.data])
            Email = db.fetchone()
            # close connection
            db.close()
            if Email:
                raise ValidationError(
                    'This Email ID is taken. Please select another one.')
