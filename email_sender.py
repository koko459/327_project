import os # import the os library
from email.message import EmailMessage # this library will allow us to send email messages
import ssl # import the ssl library
import smtplib # import the smtplib library
import serial # import the serial library

email_sender = ' ' # type in your email
email_password = os.environ.get("EMAIL_PASSWORD") # this will fetch your pasword
email_receiver = ' ' # type in the email of the person you want it to be sent to
subject = 'Fire Alert' # set the subject of the email
body = '''
A fire has possibly been detected. MAYDAY!!! SOS!!!! The authorities have been alerted... jk, we don't know how to do that but here's some info to help: 
''' # set the begininng of the body message

serialMessage = serial.Serial('COM15', 19200, timeout=1) # tell it which serial terminal to read and at what baud rate
context = ssl.create_default_context() # initialize the context variable
counter = 0 # intialize counter as 0 

while True:
    serial_message_data = serialMessage.readline() # this will begin to read the message from the selected serial port
    body_message = body + ' ' + serial_message_data # this will compose the body of the email
    em = EmailMessage() # intializes the em variable
    em['From'] = email_sender # sets the from
    em['To'] = email_receiver # sets the to
    em['Subject'] = subject # sets the subject line
    em.set_content(body_message) # makes the body_message the content of the email
    if ((len(serial_message_data) >= 60) and counter == 0): 
        # if the message is long enough and a message hasn't been sent before
        with smtplib.SMTP_SSL('smtp.gmail.com',465,context=context) as smtp: 
            smtp.login(email_sender, email_password) # login into your email
            smtp.sendmail(email_sender,email_receiver,em.as_string()) # send the email from the desired email to the other email
            counter = counter + 1 # increment the counter