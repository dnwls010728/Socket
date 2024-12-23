CREATE TABLE IF NOT EXISTS users (
    user_pk INT AUTO_INCREMENT PRIMARY KEY,
    user_id VARCHAR(64) NOT NULL UNIQUE,
    user_password VARCHAR(64) NOT NULL,
    user_money INT NOT NULL
);

CREATE TABLE IF NOT EXISTS items (
    item_pk INT AUTO_INCREMENT PRIMARY KEY,
    item_description VARCHAR(256) NOT NULL
);

CREATE TABLE IF NOT EXISTS awards(
	award_pk INT AUTO_INCREMENT PRIMARY KEY,
    user_fk INT NOT NULL,
    award_name VARCHAR(256) NOT NULL,
    award_description VARCHAR(256) NOT NULL
);

CREATE TABLE IF NOT EXISTS characters(
	character_pk INT AUTO_INCREMENT PRIMARY KEY,
    user_fk INT NOT NULL,
    character_name VARCHAR(256) NOT NULL,
    character_description VARCHAR(256) NOT NULL
);

CREATE TABLE IF NOT EXISTS weapons(
	weapon_pk INT AUTO_INCREMENT PRIMARY KEY,
    user_fk INT NOT NULL,
    weapon_name VARCHAR(256) NOT NULL,
    weapon_description VARCHAR(256) NOT NULL
);