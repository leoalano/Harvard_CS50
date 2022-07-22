# CASA BITCOIN
#### Video Demo:  <https://youtu.be/hHDhKNiTkpE>
#### Description: A Web application platform for Bitcoin 'Hodlers' to manage and increase their holdings. A Bitcoin-only Exchange. 


#### A little background
Leonardo Alano (Leo) is obsessed by Bitcoin and he sometimes helps people make their Bitcoin purchases. Leo wanted to build a tool that could help him automate and organize this work while making a prototype for a Bitcoin-only Exchange. 

#### First Steps
Leo decided to collaborate with Jake Rayapen, a fellow CS50 colleague as they thought it would be really helpful to learn to code in a collaborative form, using Git and Github. 


We set up a meeting and we drew up a plan that included the features we wanted to build, where we would work (VS Code, Trello, and GitHub), our deadlines (we had 12 days), and the roles we would take (which based on mentor conversations, we decided to work without roles, just solving tasks). 

#### Config
One of the main differences about working outside the CS50 IDE and in a new project is learning to config your machine. I learned about Virtual environments, pip installing, VS Code, Git and GitHub, configuring flask, etc. 

#### Files
##### Database 
This contains the file of the Database. We used SQLite3 and created 3 tables. A users table, a history table, and a bank details table.  

##### Static
Here it contains the logos we designed for the Bitcoin Exchange (actually we got some help for the logo), and a folder for the pictures for the profile. We also have our layout.css where all CSS work is concentrated. 

##### Templates
Here we have our HTML templates, we used a lot of Jinja templates to make it dynamic. Below a description of each template. 
1. account.html: page where user sees his/her profile.
2. admin_edit_transaction: page where the admin edits a transaction. 
3. admin.html: the main page of the admin that shows a list of transactions from all users. 
4. bankdetails.html: When a user makes a transaction and the platform doesn't have the user's bank details, it opens up a form for the user to input details. 
5. contactus.html: simple page with contact email of the platform.
6. error.html: template for a error page that receives a message on template. 
7. history.html: displays to the user their transaction history. 
8. holdings.html: displays to the user their balances in BTC, Dollars, and Reais. 
9. index: main page where there are 2 API's. One is a live graph of the price of Bitcoin in Reais and Dollars, you can see it in 1h, 24h, 7 days, 1 month, etc. The other one is an API that collects news from the internet and puts it into a dictionary with a bunch of filters. It's organized so it shows news realted to Bitcoin but NOT Ethereum or Doge or Solana that happened in the last 24 hours (so always fresh) in English. 
10. layout.html: here is the html basic layout that we extend to the other pages. Includes nav bar and different buttons depending if user is logged in or not. 
11. login.html: page where user can login with input fields of email and password. Passwords are hashed in our databased for security. 
12. market.html: page where user can make orders for the admin to aprove/confirm. Deposit + Buy, Sell, and withdrawal orders can be made here in the amount of Reais. The transaction becomes pending and admin can confirm it later (within 24 hours) so the balance appears in holdings.html. 
13. register.html: user can register themselves here. Their information is transferred to the database. 

##### forms.py
Forms.py concentrate a few forms and validators refering to the registrationform, login and account update. 

##### models.py
In models.py we have the user object. User object is used to load user in, login user, logout user, and keep track of current user.

##### requirements.txt
Here we have a list of modules, libraries and packages that are required to have installed to run the code. 

##### route.py
Here we have the main file of the project with over 700 lines. The 'brain' of the code. Here are the routes and logic necessary for each action done by the user or admin. It processes form data and interacts with the database. It also prepares all data shown in the templates.

##### run.py
This returns the globals dictionary of the executed module. The module is located using a standard import mechanism and then executed on a fresh module namespace.


#### Choices
We had multiple meetings before and during the project (and hopefully afterwards too). During the project we were in close contact via whatsapp daily and zoom calls once every 2 days. 
We would spend time discussing ideas and creating tasks on trello. We opted for example to ditch username and use email as the username as users would have to remember one less thing to enter the platform. 
We gave each other space and autonomy to work and if there were critical points we would raise it. We for example opted to change where to send a user after an error. Initially we would send them to a html error page but found to be cleaner and simpler for the user to have flash messages as feedback for a lot of the errors (and confirmations). 

Overall I think it was an extremely fruitful collaboration. 

Thank you. This was CS50. 
