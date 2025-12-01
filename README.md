# RpGB
유한대학교 2025년 졸업 작품 프로젝트

## 프로젝트 개요
자체엔진을 이용한 MMORPG 기반의 로그라이크 게임입니다.

## 개발 환경
![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![MySQL](https://img.shields.io/badge/mysql-4479A1.svg?style=for-the-badge&logo=mysql&logoColor=white)
![Rider](https://img.shields.io/badge/Rider-000000.svg?style=for-the-badge&logo=Rider&logoColor=white&color=black&labelColor=crimson)

## 라이브러리
* [Box2D](https://github.com/erincatto/box2d) - 물리엔진
* [yaml-cpp](https://github.com/jbeder/yaml-cpp) - YAML 파일 파싱
* [RTTR](https://github.com/rttrorg/rttr) - 리플렉션
* [mysql-connector-cpp](https://github.com/mysql/mysql-connector-cpp) - MySQL 데이터베이스 연결
* [FMOD](https://www.fmod.com/) - 사운드
* [tmxlite](https://github.com/fallahn/tmxlite) - Tiled 파일 파싱
* WinAPI - 윈도우 시스템 프로그래밍
* DirectX 11 - 그래픽

## 명명규칙(공통)
* [구글 C++ 명명규칙](https://google.github.io/styleguide/cppguide.html#General_Naming_Rules)
* [Commit 작성 규칙](https://cocoon1787.tistory.com/708)
* [Branch 생성 규칙](https://velog.io/@kim-jaemin420/Git-branch-naming)

## DB
<image src="./Images_README/socket_db.png"></impage>

* account_info - 유저 별 계정 정보 테이블
* character_info - 유저 별 캐릭터 정보 테이블
* inventory_item_info - 캐릭터 별 인벤토리 내 아이템 정보 테이블
* key_map_info - 캐릭터 별 퀵슬롯 정보 테이블
* skill_info - 캐릭터 별 스킬 정보 테이블
* card_offer_info, card_offer_item_info, card_choice_info - 캐릭터 별 레벨업 시 선정된 카드 및 선택된 카드 정보 테이블
* v_character_card_choice - 캐릭터 별 선택된 카드 통계 뷰
* drop_info - 몬스터 별 아이템 드랍 정보 테이블
* shop_info, shop_item_info - NPC 상점 아이템 데이터 테이블

## 결과물

## 기여자
| 담당자 | 업무 | GitHub | 이메일 |
| :--- | :---: | :---: | :---: |
| 최우진 | 엔진, 클라이언트, 서버 | [dnwls010728](https://github.com/dnwls010728) | dnwls010728@gmail.com
| 이태양 | 서버, 클라이언트 | [TaeYang](https://github.com/TaeYang112) | |
| 위세영 | 리소스, 데이터 |  [Wiseyoung](https://github.com/twozone) |  |
| 이영민 | 데이터 |  [ymlee203](https://github.com/ymlee203) |  |
| 윤이준 | 맵 | [yoonleejun](https://github.com/yoonleejun) |  |

## 느낀점 및 후기
|  |  |
| :--- | :---: |
| 최우진 |  |
| 위세영 | 팀원간의 소통이 잘 안되서 작업 분배가 잘 안되기도 하고 시간 많이 남았다고 여유부리다가 막판에 고생을 해서 작업물이 생각보다 안나온 것 같아서 아쉽다. 하지만 항상 게임을 즐기다가 게임 프로젝트를 진행해보니 게임사의 입장을 어느정도 이해하게 되는 프로젝트였다. |
| 이영민 |  |
| 윤이준 | C++와 DirectX로 RPG·PvP 게임을 직접 구현하며 렌더링부터 전투 시스템까지 많은 기술적 도전을 경험했다. 전시에서는 조작감과 전투의 재미에 대한 긍정적인 반응을 받으며 개발 과정의 보람을 느꼈다. 이번 프로젝트를 통해 협업 능력과 엔진 없이 시스템을 구축하는 개발 역량을 크게 성장시킬 수 있었다. |
