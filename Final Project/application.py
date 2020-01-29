import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session, url_for
from flask_session import Session
from tempfile import mkdtemp
from datetime import datetime
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
db = SQL("sqlite:///studentshub.db")

# Make sure API key is set
# if not os.environ.get("API_KEY"):
#    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    questions = db.execute("SELECT * from questions;")
    answers = db.execute("SELECT * from answers;")
    return render_template("main.htm", name=session["user_name"], questions=questions, q_length=len(questions), answers=answers, a_length=len(answers))


@app.route("/questions", methods=["GET"])
@login_required
def get_questions():
    rows = db.execute("SELECT * from questions;")
    return render_template("questions.html", questions=rows, length=len(rows))


@app.route("/question", methods=["POST"])
@login_required
def insert_question():
    q = request.form.get("question")
    now = datetime.now()
    current_time = now.strftime("%Y-%m-%d %H:%M:%S")
    subject = request.form.get("genre")
    stage = request.form.get("acyear")
    if not q or not subject or not stage:
        flash("Invalid input", "danger")
        return redirect("/")
    else:
        db.execute("INSERT into questions (question, Author, date, genre, academicyear) VALUES (:question, :name, :d, :genre, :acyear)",
        question=q, name=session["user_name"], d=current_time, genre=subject, acyear=stage)
        flash("Question has been shoutouted to all students in ur shcool phase", "success")
    return redirect("/")


@app.route("/qustions/filtered/subject/<string:subject>/academic year/<string:acyear>", methods=["GET"])
@login_required
def get_filtered_questions(subject, acyear):
    rows = db.execute("SELECT * from questions WHERE genre=:subject and academicyear=:acyear;",
    subject=subject, acyear=acyear)
    return render_template("filtered_questions.html", questions=rows, length=len(rows))


@app.route("/question/<int:id>")
@login_required
def get_question(id):
    question = db.execute("SELECT * from questions WHERE id=:id;", id=id)
    return render_template("question.html", question=question[0])


@app.route("/searchby", methods=["POST"])
@login_required
def searchby():
    acyear = request.form.get("acyear")
    subject = request.form.get("subject")
    if not acyear or not subject:
        flash("Invalid search!", "danger")
        return redirect("/questions")
    return redirect(url_for('get_filtered_questions', acyear=acyear, subject=subject))


@app.route("/answers", methods=["GET"])
@login_required
def get_answers():
    userid=session["user_id"]
    answers = db.execute("SELECT * from answers WHERE userid=:userid", userid=userid)
    return render_template("answers.html", answers=answers, length=len(answers))

@app.route("/answer/<int:qid>", methods=["POST"])
@login_required
def insert_answer(qid):
    userid=session["user_id"]
    a = request.form.get("answer")
    db.execute("INSERT INTO answers (answer, questionid, userid) values(:answer, :questionid, :userid)",
    answer=a, questionid=qid, userid=userid)
    flash("You successfully answered a question", "success")
    return redirect("/")


@app.route("/verify/answer/<int:aid>")
@login_required
def verify_answer(aid):
    db.execute("UPDATE answers set verified=1 WHERE id=:aid;", aid=aid)
    return redirect("/")


@app.route("/profile")
@login_required
def profile():
    id=session["user_id"]
    profile = db.execute("SELECT * from users WHERE id=:id;", id=id)
    num_of_qu = db.execute("SELECT COUNT(question) from questions where id=:id;", id=id)
    return render_template("profile.htm", profile=profile[0], n=num_of_qu[0]["COUNT(question)"])

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        session.clear()
        # Ensure username was submitted
        if not request.form.get("username"):
            flash("must provide username", "danger")
            return redirect("/login")

        # Ensure password was submitted
        elif not request.form.get("password"):
            flash("must provide password", "danger")
            return redirect("/login")

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            flash("Invalid username or password", "danger")
            return redirect("/login")

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]
        session["user_name"] = rows[0]["username"]
        # Redirect user to home page
        # return render_template("main.htm", name=session["user_name"])
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


@app.route("/student/register", methods=["GET", "POST"])
def student_register():
    """Register user"""
    if request.method == "POST":
        name = request.form.get("student")
        password = request.form.get("password")
        repeated_password = request.form.get("rpassword")
        email = request.form.get("email")
        acyear = request.form.get("acyear")
        rows = db.execute("SELECT * FROM students where username=:username", username=name.lower())
        if len(rows) > 0:
            flash(f"Username '{name}' already exists!", 'danger')
            return redirect("/student/register")
        elif password != repeated_password:
            flash("Passwords don't match!", 'danger')
            return redirect("/student/register")
        else:
            db.execute("INSERT INTO students(username, hash, email, academicyear) values(:name, :password, :mail, :ayear)",
             name=name, password=generate_password_hash(password), mail=email, ayear=acyear)
            db.execute("INSERT INTO users(username, hash, email, academicyear) values(:name, :password, :mail, :ayear)",
             name=name, password=generate_password_hash(password), mail=email, ayear=acyear)
            flash("Account created successfully", 'success')
        return redirect("/login")
    else:
        return render_template("std_register.htm")

@app.route("/instructor/register")
def instructor_register():
    return render_template("inst_register.htm")

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
