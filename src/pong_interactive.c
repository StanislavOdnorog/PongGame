#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* -------------------- CONSTANTS --------------------- */
// Field sizes definition
#define FIELD_HOR 80
#define FIELD_VER 25
#define FIELD_VER_HALF 12
#define FIELD_HOR_HALF 39

// Rackets coordinates definition
#define RACKET_L_X 10
#define RACKET_R_X 70

// Score points definition
#define SCORE_L_X 27
#define SCORE_R_X 50
#define SCORE_Y 5
#define MAX_POINT 21

/* -------------------- FUNCTIONS -------------------- */
// Functions for game logic
int start_game();
int add_score(int score);
int is_end(int score);

// Functions for render
void render_screen(int score_l, int score_r, int racket_l_y, int racket_r_y, int ball_x, int ball_y);
int is_hor_wall(int y);
int is_ver_wall(int x);
int is_score_l(int x, int y);
int is_score_r(int x, int y);
int is_ball(int x, int y, int ball_x, int ball_y);
int is_middle(int x, int y);
int is_racket_l(int x, int y, int racket_l_y);
int is_racket_r(int x, int y, int racket_r_y);

// Functions for collision
int get_racket_l_collision_dir_x(int racket_l_y, int ball_x, int ball_y, int ball_direction_x);
int get_racket_l_collision_dir_y(int racket_l_y, int ball_x, int ball_y, int ball_direction_y);
int get_racket_r_collision_dir_x(int racket_r_y, int ball_x, int ball_y, int ball_direction_x);
int get_racket_r_collision_dir_y(int racket_r_y, int ball_x, int ball_y, int ball_direction_y);
int is_wall_collision(int y);
int is_racket_wall_collision_l_up(int racket_l_y);
int is_racket_wall_collision_l_down(int racket_l_y);
int is_racket_wall_collision_r_up(int racket_r_y);
int is_racket_wall_collision_r_down(int racket_r_y);
int is_goal_racket_l(int x);
int is_goal_racket_r(int x);

// Functions for movement
int move_racket_l_up(char input, int racket_l_y);
int move_racket_l_down(char input, int racket_l_y);
int move_racket_r_up(char input, int racket_r_y);
int move_racket_r_down(char input, int racket_r_y);
int ch_hor_dir(int ball_direction_x);
int move_ball_x(int ball_x, int ball_direction);
int move_ball_y(int ball_y, int ball_direction);
int reset_ball_x();
int reset_ball_y();
int reset_ball_dir_x();
int reset_ball_dir_y();
int reset_racket();

/* -------------------- MAIN -------------------- */
int main() {
    initscr();
    noecho();
    cbreak();
    nodelay(stdscr, TRUE);

    if (start_game() == 1) {
        clear();
        printw(
            "         _                                  __                     "
            "        \n");
        printw(
            "        (_ )                               /' )                    "
            "        \n");
        printw(
            " _ _    | |    _ _  _   _    __   _ __    (_, |    _   _   _    _  "
            "   ___  \n");
        printw(
            "( '_`\\  | |  /'_` )( ) ( ) /'__`\\( '__)     | |   ( ) ( ) ( ) "
            "/'_`\\ /' _ `\\\n");
        printw(
            "| (_) ) | | ( (_| || (_) |(  ___/| |        | |   | \\_/ \\_/ |( "
            "(_) )| ( ) |\n");
        printw(
            "| ,__/'(___)`\\__,_)`\\__, |`\\____)(_)        (_)   "
            "`\\___x___/'`\\___/'(_) (_)\n");
        printw("| |                ( )_| |\n");
        printw("(_)                `\\___/'\n");
    } else {
        clear();
        printw(
            "        _                                    __                    "
            "           \n");
        printw(
            "       (_ )                                /'__`\\                 "
            "            \n");
        printw(
            " _ _    | |    _ _  _   _    __   _ __    (_)  ) )    _   _   _    "
            "_     ___  \n");
        printw(
            "( '_`\\  | |  /'_` )( ) ( ) /'__`\\( '__)      /' /    ( ) ( ) ( ) "
            "/'_`\\ /' _ `\n");
        printw(
            "| (_) ) | | ( (_| || (_) |(  ___/| |       /' /( )   | \\_/ \\_/ "
            "|( (_) )| ( ) |\n");
        printw(
            "| ,__/'(___)`\\__,_)`\\__, |`\\____)(_)      (_____/'   "
            "`\\___x___/'`\\___/'(_) (_)\n");
        printw("| |                ( )_| |\n");
        printw("(_)                `\\___/'\n");
    }

    char temp;
    scanf("%c", &temp);
    endwin();
    return 0;
}

/* -------------------- Functions for game logic -------------------- */
int start_game() {
    int racket_l_y = FIELD_VER_HALF;
    int racket_r_y = FIELD_VER_HALF;
    int ball_x = FIELD_HOR_HALF;
    int ball_y = (rand() % 22) + 2;
    int ball_direction_x = reset_ball_dir_x();
    int ball_direction_y = reset_ball_dir_y();
    int score_l = 0;
    int score_r = 0;
    srand(time(NULL));

    while (1) {
        system("sleep 0.08");
        char input;
        clear();
        render_screen(score_l, score_r, racket_l_y, racket_r_y, ball_x, ball_y);
        input = getch();
        if (input == 'q') {
        return 1;
  }

        if (!is_racket_wall_collision_l_up(racket_l_y)) {
            racket_l_y = move_racket_l_up(input, racket_l_y);
        }
        if (!is_racket_wall_collision_l_down(racket_l_y)) {
            racket_l_y = move_racket_l_down(input, racket_l_y);
        }
        if (!is_racket_wall_collision_r_up(racket_r_y)) {
            racket_r_y = move_racket_r_up(input, racket_r_y);
        }
        if (!is_racket_wall_collision_r_down(racket_r_y)) {
            racket_r_y = move_racket_r_down(input, racket_r_y);
        }

        ball_direction_x = get_racket_l_collision_dir_x(racket_l_y, ball_x, ball_y, ball_direction_x);
        ball_direction_y = get_racket_l_collision_dir_y(racket_l_y, ball_x, ball_y, ball_direction_y);
        ball_direction_x = get_racket_r_collision_dir_x(racket_r_y, ball_x, ball_y, ball_direction_x);
        ball_direction_y = get_racket_r_collision_dir_y(racket_r_y, ball_x, ball_y, ball_direction_y);

        if (is_wall_collision(ball_y)) {
            ball_direction_y = ch_hor_dir(ball_direction_y);
        }

        ball_x = move_ball_x(ball_x, ball_direction_x);
        ball_y = move_ball_y(ball_y, ball_direction_y);

        if (is_goal_racket_l(ball_x)) {
            score_l = add_score(score_l);
            ball_x = reset_ball_x();
            ball_y = reset_ball_y();
            racket_l_y = reset_racket();
            racket_r_y = reset_racket();
            ball_direction_x = reset_ball_dir_x();
            ball_direction_y = reset_ball_dir_y();
        }

        if (is_goal_racket_r(ball_x)) {
            score_r = add_score(score_r);
            ball_x = reset_ball_x();
            ball_y = reset_ball_y();
            racket_l_y = reset_racket();
            racket_r_y = reset_racket();
            ball_direction_x = reset_ball_dir_x();
            ball_direction_y = reset_ball_dir_y();
        }

        if (is_end(score_l)) {
            return 1;
        }
        if (is_end(score_r)) {
            return 0;
        }
    }
}

int add_score(int score) { return ++score; }

int is_end(int score) { return score < MAX_POINT ? 0 : 1; }

/* -------------------- Functions for render -------------------- */
void render_screen(int score_l, int score_r, int racket_l_y, int racket_r_y, int ball_x, int ball_y) {
    int x, y;
    for (y = 0; y <= FIELD_VER; y++) {
        for (x = 0; x <= FIELD_HOR; x++) {
            if (is_hor_wall(y)) {
                printw("#");
            } else if (is_ver_wall(x)) {
                printw("#");
            } else if (is_score_l(x, y)) {
                printw("%d", score_l);
                if (score_l >= 10) {
                    x++;
                }
            } else if (is_score_r(x, y)) {
                printw("%d", score_r);
                if (score_r >= 10) {
                    x++;
                }
            } else if (is_ball(x, y, ball_x, ball_y)) {
                printw("*");
            } else if (is_middle(x, y)) {
                printw("'");
            } else if (is_racket_r(x, y, racket_r_y)) {
                printw("|");
            } else if (is_racket_l(x, y, racket_l_y)) {
                printw("|");
            } else {
                printw(" ");
            }
        }
        printw("\n");
    }
}

int is_hor_wall(int y) {
    if (y == FIELD_VER || y == 0) {
        return 1;
    } else {
        return 0;
    }
}

int is_ver_wall(int x) {
    if (x == FIELD_HOR || x == 0) {
        return 1;
    } else {
        return 0;
    }
}
int is_score_l(int x, int y) {
    if (x == SCORE_L_X && y == SCORE_Y) {
        return 1;
    } else {
        return 0;
    }
}

int is_score_r(int x, int y) {
    if (x == SCORE_R_X && y == SCORE_Y) {
        return 1;
    } else {
        return 0;
    }
}

int is_ball(int x, int y, int ball_x, int ball_y) {
    if ((x == ball_x) && (y == ball_y)) {
        return 1;
    } else {
        return 0;
    }
}

int is_middle(int x, int y) {
    if ((x == FIELD_HOR_HALF) && (y > 1) && (y < FIELD_VER - 1)) {
        return 1;
    } else {
        return 0;
    }
}

int is_racket_l(int x, int y, int racket_l_y) {
    if (x != RACKET_L_X) return 0;
    if (y == racket_l_y) return 1;
    if (y == racket_l_y + 1) return 1;
    if (y == racket_l_y - 1)
        return 1;
    else
        return 0;
}

int is_racket_r(int x, int y, int racket_r_y) {
    if (x != RACKET_R_X) return 0;
    if (y == racket_r_y) return 1;
    if (y == racket_r_y + 1) return 1;
    if (y == racket_r_y - 1)
        return 1;
    else
        return 0;
}

/* -------------------- Functions for collision -------------------- */
int get_racket_l_collision_dir_x(int racket_l_y, int ball_x, int ball_y, int ball_direction_x) {
    if (ball_x != RACKET_L_X + 1) return ball_direction_x;
    if (ball_y == racket_l_y) return -ball_direction_x;
    if (ball_y == racket_l_y + 1 || ball_y == racket_l_y + 2) return -ball_direction_x;
    if (ball_y == racket_l_y - 1 || ball_y == racket_l_y - 2)
        return -ball_direction_x;
    else
        return ball_direction_x;
}

int get_racket_l_collision_dir_y(int racket_l_y, int ball_x, int ball_y, int ball_direction_y) {
    if (ball_x != RACKET_L_X + 1) return ball_direction_y;
    if (ball_y == racket_l_y) return 0;
    if (ball_y == racket_l_y + 1 || ball_y == racket_l_y + 2) return 1;
    if (ball_y == racket_l_y - 1 || ball_y == racket_l_y - 2)
        return -1;
    else
        return ball_direction_y;
}

int get_racket_r_collision_dir_x(int racket_r_y, int ball_x, int ball_y, int ball_direction_x) {
    if (ball_x != RACKET_R_X - 1) return ball_direction_x;
    if (ball_y == racket_r_y) return -ball_direction_x;
    if (ball_y == racket_r_y + 1 || ball_y == racket_r_y + 2) return -ball_direction_x;
    if (ball_y == racket_r_y - 1 || ball_y == racket_r_y - 2)
        return -ball_direction_x;
    else
        return ball_direction_x;
}

int get_racket_r_collision_dir_y(int racket_r_y, int ball_x, int ball_y, int ball_direction_y) {
    if (ball_x != RACKET_R_X - 1) return ball_direction_y;
    if (ball_y == racket_r_y) return 0;
    if (ball_y == racket_r_y + 1 || ball_y == racket_r_y + 2) return 1;
    if (ball_y == racket_r_y - 1 || ball_y == racket_r_y - 2)
        return -1;
    else
        return ball_direction_y;
}

int is_wall_collision(int y) {
    if ((y == 1) || (y == FIELD_VER - 1)) {
        return 1;
    } else {
        return 0;
    }
}

int is_racket_wall_collision_l_up(int racket_l_y) {
    if (racket_l_y == 2) {
        return 1;
    } else {
        return 0;
    }
}

int is_racket_wall_collision_l_down(int racket_l_y) {
    if (racket_l_y == FIELD_VER - 2) {
        return 1;
    } else {
        return 0;
    }
}

int is_racket_wall_collision_r_up(int racket_r_y) {
    if (racket_r_y == 2) {
        return 1;
    } else {
        return 0;
    }
}

int is_racket_wall_collision_r_down(int racket_r_y) {
    if (racket_r_y == FIELD_VER - 2) {
        return 1;
    } else {
        return 0;
    }
}

int is_goal_racket_l(int x) {
    if (x == FIELD_HOR) {
        return 1;
    }
    return 0;
}

int is_goal_racket_r(int x) {
    if (x == 0) {
        return 1;
    }
    return 0;
}

/* -------------------- Functions for movement -------------------- */
int move_racket_l_up(char input, int racket_l_y) {
    if (input == 'A' || input == 'a') {
        return --racket_l_y;
    } else {
        return racket_l_y;
    };
}

int move_racket_l_down(char input, int racket_l_y) {
    if (input == 'Z' || input == 'z') {
        return ++racket_l_y;
    } else {
        return racket_l_y;
    };
}

int move_racket_r_up(char input, int racket_r_y) {
    if (input == 'K' || input == 'k') {
        return --racket_r_y;
    } else {
        return racket_r_y;
    };
}

int move_racket_r_down(char input, int racket_r_y) {
    if (input == 'M' || input == 'm') {
        return ++racket_r_y;
    } else {
        return racket_r_y;
    };
}

int ch_hor_dir(int ball_direction_x) { return -ball_direction_x; }

int move_ball_x(int ball_x, int ball_direction_x) { return ball_x + ball_direction_x; }

int move_ball_y(int ball_y, int ball_direction_y) { return ball_y + ball_direction_y; }

int reset_ball_x() { return FIELD_HOR_HALF; }

int reset_ball_y() { return (rand() % 22) + 2; }

int reset_ball_dir_x() { return ((rand() % 2) - 1) ? 1 : -1; }
int reset_ball_dir_y() { return (rand() % 3) - 1; }

int reset_racket() { return FIELD_VER_HALF; }
