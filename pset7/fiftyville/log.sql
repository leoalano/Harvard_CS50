-- Keep a log of any SQL queries you execute as you solve the mystery.
-- All you know is that the theft took place on July 28, 2020 and that it took place on Chamberlin Street.

-- overview. Will follow hint to have a llok at the crime scene reports
.schema

-- All reports organized by date to have a wide understanding of daily reports. I'll copy the report of the
-- theft here: "2020 | 7 | 28 | Theft of the CS50 duck took place at 10:15am at the Chamberlin Street
-- courthouse. Interviews were conducted today with three witnesses who were present at the time — each of
-- their interview transcripts mentions the courthouse."
SELECT year, month, day, description FROM crime_scene_reports;

-- It seems obvious to check the interviews of the witnesses.
-- 161 | Ruth | Sometime within ten minutes of the theft, I saw the thief get into a car in the courthouse
-- parking lot and drive away. If you have security footage from the courthouse parking lot, you might want
-- to look for cars that left the parking lot in that time frame.
-- 162 | Eugene | I don't know the thief's name, but it was someone I recognized. Earlier this morning,
-- before I arrived at the courthouse, I was walking by the ATM on Fifer Street and saw the thief there
-- withdrawing some money.
-- 163 | Raymond | As the thief was leaving the courthouse, they called someone who talked to them for less
-- than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out
-- of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the
-- flight ticket.
SELECT id, name, transcript FROM interviews WHERE day=28 AND year=2020 AND month=07 AND transcript LIKE '%courthouse%';

-- 3 clues. I'l'start with Ruth's clue. The thief is inside one of these plates. The plate may belong to
-- thief or accomplice.
-- 260 | 2020 | 7 | 28 | 10 | 16 | exit | 5P2BI95
-- 261 | 2020 | 7 | 28 | 10 | 18 | exit | 94KL13X
-- 262 | 2020 | 7 | 28 | 10 | 18 | exit | 6P58WS2
-- 263 | 2020 | 7 | 28 | 10 | 19 | exit | 4328GD8
-- 264 | 2020 | 7 | 28 | 10 | 20 | exit | G412CB7
-- 265 | 2020 | 7 | 28 | 10 | 21 | exit | L93JTIZ
-- 266 | 2020 | 7 | 28 | 10 | 23 | exit | 322W7JE
-- 267 | 2020 | 7 | 28 | 10 | 23 | exit | 0NTHK55
SELECT * FROM courthouse_security_logs WHERE day=28 AND year=2020 AND month=07 AND hour=10 AND minute<30 AND minute>15;

-- find name of plate owner
SELECT * FROM people WHERE license_plate IN (SELECT license_plate FROM courthouse_security_logs WHERE day=28 AND year=2020 AND month=07 AND hour=10 AND minute<30 AND minute>15);

-- We have some suspects. one of these people is the owner of the car that the thief drove.
-- 221103 | Patrick | (725) 555-4692 | 2963008352 | 5P2BI95
-- 243696 | Amber | (301) 555-4174 | 7526138472 | 6P58WS2
-- 396669 | Elizabeth | (829) 555-5269 | 7049073643 | L93JTIZ
-- 398010 | Roger | (130) 555-0289 | 1695452385 | G412CB7
-- 467400 | Danielle | (389) 555-5198 | 8496433585 | 4328GD8
-- 514354 | Russell | (770) 555-1861 | 3592750733 | 322W7JE
-- 560886 | Evelyn | (499) 555-9472 | 8294398571 | 0NTHK55
-- 686048 | Ernest | (367) 555-5533 | 5773159633 | 94KL13X

-- I'm going to follow clues from second witness. Eugene.
-- 162 | Eugene | I don't know the thief's name, but it was someone I recognized. Earlier this morning,
-- before I arrived at the courthouse, I was walking by the ATM on Fifer Street and saw the thief there
-- withdrawing some money.
SELECT * FROM atm_transactions WHERE day=28 AND year=2020 AND month=07 AND atm_location='Fifer Street' AND transaction_type='withdraw';

-- id | account_number | year | month | day | atm_location | transaction_type | amount
-- 246 | 28500762 | 2020 | 7 | 28 | Fifer Street | withdraw | 48
-- 264 | 28296815 | 2020 | 7 | 28 | Fifer Street | withdraw | 20
-- 266 | 76054385 | 2020 | 7 | 28 | Fifer Street | withdraw | 60
-- 267 | 49610011 | 2020 | 7 | 28 | Fifer Street | withdraw | 50
-- 269 | 16153065 | 2020 | 7 | 28 | Fifer Street | withdraw | 80
-- 288 | 25506511 | 2020 | 7 | 28 | Fifer Street | withdraw | 20
-- 313 | 81061156 | 2020 | 7 | 28 | Fifer Street | withdraw | 30
-- 336 | 26013199 | 2020 | 7 | 28 | Fifer Street | withdraw | 35


-- I need to find a list of names of the account number.
SELECT * FROM people JOIN bank_accounts ON bank_accounts.person_id=people.id WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE day=28 AND year=2020 AND month=07 AND atm_location='Fifer Street' AND transaction_type='withdraw');

-- These are the people that withdrew money around the time the witness saw.
-- id | name | phone_number | passport_number | license_plate | account_number | person_id | creation_year
-- 686048 | Ernest | (367) 555-5533 | 5773159633 | 94KL13X | 49610011 | 686048 | 2010
-- 514354 | Russell | (770) 555-1861 | 3592750733 | 322W7JE | 26013199 | 514354 | 2012
-- 458378 | Roy | (122) 555-4581 | 4408372428 | QX4YZN3 | 16153065 | 458378 | 2012
-- 395717 | Bobby | (826) 555-1652 | 9878712108 | 30G67EN | 28296815 | 395717 | 2014
-- 396669 | Elizabeth | (829) 555-5269 | 7049073643 | L93JTIZ | 25506511 | 396669 | 2014
-- 467400 | Danielle | (389) 555-5198 | 8496433585 | 4328GD8 | 28500762 | 467400 | 2014
-- 449774 | Madison | (286) 555-6063 | 1988161715 | 1106N58 | 76054385 | 449774 | 2015
-- 438727 | Victoria | (338) 555-6650 | 9586786673 | 8X428L0 | 81061156 | 438727 | 2018


-- So far, the if the car owner is the thief and not the accomplice, the matching suspects would be:
-- Elizabeth, Danielle, Russel, Ernest.

-- Let's check third witness.
-- 163 | Raymond | As the thief was leaving the courthouse, they called someone who talked to them for less
-- than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out
-- of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the
-- flight ticket.
SELECT * FROM phone_calls WHERE day=28 AND year=2020 AND month=07 AND duration<60;
-- id | caller | receiver | year | month | day | duration
-- 221 | (130) 555-0289 | (996) 555-8899 | 2020 | 7 | 28 | 51
-- 224 | (499) 555-9472 | (892) 555-8872 | 2020 | 7 | 28 | 36
-- 233 | (367) 555-5533 | (375) 555-8161 | 2020 | 7 | 28 | 45
-- 251 | (499) 555-9472 | (717) 555-1342 | 2020 | 7 | 28 | 50
-- 254 | (286) 555-6063 | (676) 555-6554 | 2020 | 7 | 28 | 43
-- 255 | (770) 555-1861 | (725) 555-3243 | 2020 | 7 | 28 | 49
-- 261 | (031) 555-6622 | (910) 555-3251 | 2020 | 7 | 28 | 38
-- 279 | (826) 555-1652 | (066) 555-9701 | 2020 | 7 | 28 | 55
-- 281 | (338) 555-6650 | (704) 555-2131 | 2020 | 7 | 28 | 54

-- 221103 | Patrick | (725) 555-4692 | 2963008352 | 5P2BI95
-- 243696 | Amber | (301) 555-4174 | 7526138472 | 6P58WS2
-- 396669 | Elizabeth | (829) 555-5269 | 7049073643 | L93JTIZ
-- 398010 | Roger | (130) 555-0289 | 1695452385 | G412CB7
-- 467400 | Danielle | (389) 555-5198 | 8496433585 | 4328GD8
-- 514354 | Russell | (770) 555-1861 | 3592750733 | 322W7JE
-- 560886 | Evelyn | (499) 555-9472 | 8294398571 | 0NTHK55
-- 686048 | Ernest | (367) 555-5533 | 5773159633 | 94KL13X

SELECT caller FROM phone_calls WHERE day=28 AND year=2020 AND month=07 AND duration<60;

-- Find matching phone numbers caller and bank account owner.
SELECT DISTINCT name, phone_number FROM people JOIN phone_calls WHERE phone_number IN (SELECT caller FROM phone_calls WHERE day=28 AND year=2020 AND month=07 AND duration<60) AND phone_number IN (SELECT phone_number FROM people JOIN bank_accounts ON bank_accounts.person_id=people.id WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE day=28 AND year=2020 AND month=07 AND atm_location='Fifer Street' AND transaction_type='withdraw'));
-- name | phone_number
-- Bobby | (826) 555-1652
-- Victoria | (338) 555-6650
-- Madison | (286) 555-6063
-- Russell | (770) 555-1861
-- Ernest | (367) 555-5533


-- Find matching phone numbers receiver and bank account owner.
SELECT DISTINCT name, phone_number FROM people JOIN phone_calls WHERE phone_number IN (SELECT receiver FROM phone_calls WHERE day=28 AND year=2020 AND month=07 AND duration<60) AND phone_number IN (SELECT phone_number FROM people JOIN bank_accounts ON bank_accounts.person_id=people.id WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE day=28 AND year=2020 AND month=07 AND atm_location='Fifer Street' AND transaction_type='withdraw'));

-- the thief was the caller as there's no match between receiver and owner of bank account.
-- THIEF!!!!!
SELECT DISTINCT name, phone_number FROM people JOIN phone_calls WHERE phone_number IN (SELECT caller FROM phone_calls WHERE day=28 AND year=2020 AND month=07 AND duration<60) AND phone_number IN (SELECT phone_number FROM people JOIN bank_accounts ON bank_accounts.person_id=people.id WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE day=28 AND year=2020 AND month=07 AND atm_location='Fifer Street' AND transaction_type='withdraw'));

-- name | phone_number
-- Bobby | (826) 555-1652
-- Victoria | (338) 555-6650
-- Madison | (286) 555-6063
-- Russell | (770) 555-1861
-- Ernest | (367) 555-5533

-- the accomplice is one of these
SELECT receiver FROM phone_calls WHERE day=28 AND year=2020 AND month=07 AND duration<60;

-- name of accomplice!!!!!
SELECT name, passport_number, License_plate FROM people WHERE phone_number IN (SELECT receiver FROM phone_calls WHERE day=28 AND year=2020 AND month=07 AND duration<60);
name | passport_number | license_plate
-- James | 2438825627 | Q13SVG6
-- Larry | 2312901747 | O268ZZ0
-- Anna |  |
-- Jack | 9029462229 | 52R0Y8U
-- Melissa | 7834357192 |
-- Jacqueline |  | 43V0R5D
-- Philip | 3391710505 | GW362R6
-- Berthold |  | 4V16VO0
-- Doris | 7214083635 | M51FA04


-- Check match between receiver and bank account. No match.
SELECT name FROM people WHERE name IN (SELECT name FROM people JOIN bank_accounts ON bank_accounts.person_id=people.id WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE day=28 AND year=2020 AND month=07 AND atm_location='Fifer Street' AND transaction_type='withdraw')) AND name IN (SELECT name FROM people WHERE phone_number IN (SELECT receiver FROM phone_calls WHERE day=28 AND year=2020 AND month=07 AND duration<60));

-- Check match betwen license plate and accomplice. No match.
SELECT name FROM people WHERE name IN (SELECT name FROM people WHERE license_plate IN (SELECT license_plate FROM courthouse_security_logs WHERE day=28 AND year=2020 AND month=07 AND hour=10 AND minute<30 AND minute>15)) AND name IN (SELECT name FROM people WHERE phone_number IN (SELECT receiver FROM phone_calls WHERE day=28 AND year=2020 AND month=07 AND duration<60));

-- Check match betwen license plate and thief. RUSSEL OR ERNEST is the THIEF!!!!!
SELECT name FROM people WHERE name IN (SELECT name FROM people WHERE license_plate IN (SELECT license_plate FROM courthouse_security_logs WHERE day=28 AND year=2020 AND month=07 AND hour=10 AND minute<30 AND minute>15)) AND name IN (SELECT DISTINCT name FROM people JOIN phone_calls WHERE phone_number IN (SELECT caller FROM phone_calls WHERE day=28 AND year=2020 AND month=07 AND duration<60)) AND phone_number IN (SELECT phone_number FROM people JOIN bank_accounts ON bank_accounts.person_id=people.id WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE day=28 AND year=2020 AND month=07 AND atm_location='Fifer Street' AND transaction_type='withdraw'));

-- Check who Russel and Ernest were talking to.
-- phone number from russel
SELECT phone_number FROM people WHERE name='Russell';

--Extract details of receiver of Russel
SELECT * FROM people WHERE phone_number=(SELECT receiver FROM phone_calls WHERE day=28 AND year=2020 AND month=07 AND duration<60 AND caller=(SELECT phone_number FROM people WHERE name='Russell'));

--IF RUSSELL, THIS IS THE ACCOMPLICE
-- id | name | phone_number | passport_number | license_plate
-- 847116 | Philip | (725) 555-3243 | 3391710505 | GW362R6

-- phone number from Ernest
SELECT phone_number FROM people WHERE name='Ernest';

--Extract details of receiver of Ernest
SELECT * FROM people WHERE phone_number=(SELECT receiver FROM phone_calls WHERE day=28 AND year=2020 AND month=07 AND duration<60 AND caller=(SELECT phone_number FROM people WHERE name='Ernest'));

--IF ERNEST, THIS IS THE ACCOMPLICE
-- id | name | phone_number | passport_number | license_plate
-- 864400 | Berthold | (375) 555-8161 |  | 4V16VO0

-- Find flights after the theft. Returns a lot of options..
SELECT * FROM flights WHERE year=2020 AND month>=7 AND month<9 AND day>=28;
-- id | origin_airport_id | destination_airport_id | year | month | day | hour | minute

-- Investigate origin airport. Fiftyville's id is 8.
SELECT * FROM airports;
-- id | abbreviation | full_name | city
-- 8 | CSF | Fiftyville Regional Airport | Fiftyville

-- INFO FROM flights going out of fiftyville.
SELECT * FROM flights WHERE year=2020 AND month>=7 AND month<9 AND day>=28 AND origin_airport_id=8;
-- Returns a lot of options still. Need to narrow more.

--Flight ID from Russell
SELECT flight_id FROM passengers WHERE passport_number=(SELECT passport_number FROM people WHERE name='Russell');

-- match flight ID from suspect (flights in fiftyville) and russell.
SELECT * FROM flights WHERE year=2020 AND month>=7 AND month<9 AND day>=28 AND origin_airport_id=8 AND id IN (SELECT flight_id FROM passengers WHERE passport_number=(SELECT passport_number FROM people WHERE name='Russell'));
-- id | origin_airport_id | destination_airport_id | year | month | day | hour | minute
-- 18 | 8 | 6 | 2020 | 7 | 29 | 16 | 0
-- 54 | 8 | 5 | 2020 | 7 | 30 | 10 | 19


-- match flight ID from suspect (flights out fiftyville) and russell.
SELECT * FROM flights WHERE year=2020 AND month>=7 AND month<9 AND day>=28 AND destination_airport_id=8 AND id IN (SELECT flight_id FROM passengers WHERE passport_number=(SELECT passport_number FROM people WHERE name='Russell'));
-- id | origin_airport_id | destination_airport_id | year | month | day | hour | minute
-- 24 | 7 | 8 | 2020 | 7 | 30 | 16 | 27


--Flight ID from Ernest
SELECT flight_id FROM passengers WHERE passport_number=(SELECT passport_number FROM people WHERE name='Ernest');

-- match flight ID from suspect (flights out fiftyville) and ernest.
SELECT * FROM flights WHERE year=2020 AND month>=7 AND month<9 AND day>=28 AND origin_airport_id=8 AND id IN (SELECT flight_id FROM passengers WHERE passport_number=(SELECT passport_number FROM people WHERE name='Ernest'));

-- Destination id is 4 so he escaped to Heathrow Airport | London. The time matches as well. Russell's trips don't make sense so I think Ernest is our guy.

-- And if Enrnest is our guy, based on conclusion on line 181, Berthold is the accomplice and Ernest went to London. 

