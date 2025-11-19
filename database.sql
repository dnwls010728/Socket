create table account_info
(
    account_id int auto_increment
        primary key,
    id         varchar(100) charset utf8mb3             null,
    password   varchar(100) charset utf8mb3             null,
    logged_in  bit         default b'0'                 null,
    last_login datetime(6)                              null,
    created_at datetime(6) default CURRENT_TIMESTAMP(6) null,
    constraint account_info_pk
        unique (id)
)
    collate = utf8mb4_general_ci;

create table character_info
(
    character_id    int auto_increment
        primary key,
    account_id      int                          not null,
    name            varchar(100) charset utf8mb3 not null,
    body_color      varchar(6) default 'FFFFFF'  null,
    lv              int        default 1         not null,
    hp              int        default 350       not null,
    max_hp          int        default 350       not null,
    exp             int        default 0         not null,
    map_id          int        default 0         not null,
    last_position_x float      default 0         not null,
    last_position_y float      default 0         not null,
    color           int        default 0         not null comment '재화',
    party_id        int        default 0         not null,
    constraint character_info_account_info_account_id_fk
        foreign key (account_id) references account_info (account_id)
            on delete cascade
)
    collate = utf8mb4_general_ci;

create table card_offer_info
(
    offer_id     binary(16)                                   not null
        primary key,
    character_id int                                      not null,
    level        int                                      not null,
    status       int         default 0                    not null,
    offered_at   datetime(6) default CURRENT_TIMESTAMP(6) not null,
    constraint card_offer_info_pk_2
        unique (character_id, level),
    constraint card_offer_info_character_info_character_id_fk
        foreign key (character_id) references character_info (character_id)
            on delete cascade
);

create table card_offer_item_info
(
    offer_id binary(16)  not null,
    slot     tinyint not null,
    card_id  int     not null,
    primary key (offer_id, slot),
    constraint card_offer_item_info_pk
        unique (offer_id, card_id),
    constraint card_offer_item_info_card_offer_info_offer_id_fk
        foreign key (offer_id) references card_offer_info (offer_id)
            on delete cascade
);

create table card_choice_info
(
    offer_id       binary(16)                                   not null
        primary key,
    chosen_card_id int                                      null,
    slot           tinyint                                  null,
    chosen_at      datetime(6) default CURRENT_TIMESTAMP(6) not null,
    constraint card_choice_info_card_offer_info_offer_id_fk
        foreign key (offer_id) references card_offer_info (offer_id)
            on delete cascade,
    constraint card_choice_info_card_offer_item_info_offer_id_card_id_fk
        foreign key (offer_id, chosen_card_id) references card_offer_item_info (offer_id, card_id)
);

create table drop_info
(
    id         int auto_increment
        primary key,
    dropper_id int           not null,
    item_id    int default 0 not null comment '0일 경우 컬러',
    min_count  int default 1 not null,
    max_count  int default 1 not null,
    chance     int default 0 not null comment '최대 값 10000 = 0.01%',
    constraint drop_info_pk
        unique (item_id, dropper_id)
);

create table inventory_item_info
(
    inventory_item_id int auto_increment
        primary key,
    account_id        int default 0 not null,
    character_id      int default 0 not null,
    inventory_type    int default 0 not null,
    item_id           int default 0 not null,
    slot_id           int default 0 not null,
    count             int default 0 not null,
    constraint inventory_item_info_account_info_account_id_fk
        foreign key (account_id) references account_info (account_id)
            on delete cascade,
    constraint inventory_item_info_character_info_character_id_fk
        foreign key (character_id) references character_info (character_id)
            on delete cascade
);

create table key_map_info
(
    key_map_id   int auto_increment,
    character_id int default 0 not null,
    scancode     int default 0 not null,
    type         int default 0 not null,
    action       int default 0 not null,
    constraint key_map_info_pk
        unique (key_map_id),
    constraint key_map_info_character_info_character_id_fk
        foreign key (character_id) references character_info (character_id)
            on delete cascade
);

create table skill_info
(
    id           int auto_increment
        primary key,
    character_id int           not null,
    skill_id     int           not null,
    skill_level  int           not null,
    duration     int default 0 not null,
    start_time   int default 0 not null,
    constraint skill_info_character_info_character_id_fk
        foreign key (character_id) references character_info (character_id)
            on delete cascade
);

CREATE OR REPLACE
VIEW v_character_card_choice AS
SELECT
  o.character_id,
  o.level,
  o.offer_id                                AS offer_id_bin,         -- 바이너리 원본
  BIN_TO_UUID(o.offer_id, TRUE)             AS offer_id,             -- 사람이 읽는 UUID
  c.chosen_card_id,
  c.slot,
  c.chosen_at
FROM socket_db.card_choice_info AS c
JOIN socket_db.card_offer_info  AS o
  ON o.offer_id = c.offer_id;  -- 여기서는 변환 X (인덱스 사용)



