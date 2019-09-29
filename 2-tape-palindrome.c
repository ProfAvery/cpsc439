#include <stdio.h>
#include <stdbool.h>        // Requires C99 or later
#include <assert.h>

#define K 2
#define LENGTH 80
#define INITIAL_POSITION 5  // allow a margin so we can move left

#define TAPE(n) tape[(n)][head[(n)]]
#define SYMBOLS (gamma[])
#define DIRECTIONS (direction[])

typedef enum { A = 'a', B = 'b', BLANK = '.' } gamma;
typedef enum { L, R, N } direction;

gamma tape[K][LENGTH];
int head[K];

void tape_init(int nsymbols, gamma values[K][nsymbols])
{
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < LENGTH; j++) {
            tape[i][j] = BLANK;
        }
        head[i] = INITIAL_POSITION;
    }

    for (int i = 0; i < K; i++) {
        for (int j = 0; j < nsymbols; j++) {
            tape[i][INITIAL_POSITION + j] = values[i][j];
        }
    }
}

void print_tapes(void)
{
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < LENGTH; j++) {
            if (head[i] == j) {
                printf("v");
            } else {
                printf(" ");
            }
        }
        printf("\n");

        for (int j = 0; j < LENGTH; j++) {
            printf("%c", tape[i][j]);
        }
        printf("\n");
    }
}

void write_tape(gamma symbol[])
{
    for (int i = 0; i < K; i++) {
        int pos = head[i];
        tape[i][pos] = symbol[i];
    }
}

void move_head(direction where[])
{
    for (int i = 0; i < K; i++) {
        switch (where[i]) {
            case L:
                head[i]--;
                break;
            case R:
                head[i]++;
                break;
            case N:
                break;
            default:
                // We should never reach here
                assert(false);
        }

        assert(head[i] >= 0 && head[i] < LENGTH);
    }
}

/* Transition function from p. 143 */

bool delta(void)
{
Q0:
    // q0 a BLANK -> q0 a a R R
    if (TAPE(0) == A && TAPE(1) == BLANK) {
        write_tape(SYMBOLS{A, A});
        move_head(DIRECTIONS{R, R});
        goto Q0;
    }

    // q0 b BLANK -> q0 b b R R
    if (TAPE(0) == B && TAPE(1) == BLANK) {
        write_tape(SYMBOLS{B, B});
        move_head(DIRECTIONS{R, R});
        goto Q0;
    }

    // q0 BLANK BLANK -> q1 BLANK BLANK L L
    if (TAPE(0) == BLANK && TAPE(1) == BLANK) {
        write_tape(SYMBOLS{BLANK, BLANK});
        move_head(DIRECTIONS{L, L});
        goto Q1;
    }

Q1:
    // q1 a a -> q1 a a L N
    if (TAPE(0) == A && TAPE(1) == A ) {
        write_tape(SYMBOLS{A, A});
        move_head(DIRECTIONS{L, N});
        goto Q1;
    }

    // q1 a b -> q1 a b L N
    if (TAPE(0) == A && TAPE(1) == B) {
        write_tape(SYMBOLS{A, B});
        move_head(DIRECTIONS{L, N});
        goto Q1;
    }

    // q1 b a -> q1 b a L N
    if (TAPE(0) == B && TAPE(1) == A) {
        write_tape(SYMBOLS{B, A});
        move_head(DIRECTIONS{L, N});
        goto Q1;
    }

    // q1 b b -> q1 b b L N
    if (TAPE(0) == B && TAPE(1) == B) {
        write_tape(SYMBOLS{B, B});
        move_head(DIRECTIONS{L, N});
        goto Q1;
    }

    // q1 BLANK a -> q2 BLANK a R N
    if (TAPE(0) == BLANK && TAPE(1) == A) {
        write_tape(SYMBOLS{BLANK, A});
        move_head(DIRECTIONS{R, N});
        goto Q2;
    }

    // q1 BLANK b -> q2 BLANK b R N
    if (TAPE(0) == BLANK && TAPE(1) == B) {
        write_tape(SYMBOLS{BLANK, B});
        move_head(DIRECTIONS{R, N});
        goto Q2;
    }

    // q1 BLANK BLANK -> qaccept
    if (TAPE(0) == BLANK && TAPE(1) == BLANK) {
        goto QACCEPT;
    }

Q2:
    // q2 a a -> q2 a a R L
    if (TAPE(0) == A && TAPE(1) == A) {
        write_tape(SYMBOLS{A, A});
        move_head(DIRECTIONS{R, L});
        goto Q2;
    }

    // q2 a b -> qreject
    if (TAPE(0) == A && TAPE(1) == B) {
        goto QREJECT;
    }

    // q2 b a -> qreject
    if (TAPE(0) == B && TAPE(1) == A) {
        goto QREJECT;
    }

    // q2 b b -> q2 b b R L
    if (TAPE(0) == B && TAPE(1) == B) {
        write_tape(SYMBOLS{B, B});
        move_head(DIRECTIONS{R, L});
        goto Q2;
    }

    // q2 BLANK BLANK -> qaccept
    if (TAPE(0) == BLANK && TAPE(1) == BLANK) {
        goto QACCEPT;
    }

QACCEPT:
    return true;

QREJECT:
    return false;
}

#define LEN 4

int main(void)
{
    gamma input[K][LEN] = {
        { A, B, B, A },
        { BLANK, BLANK, BLANK, BLANK},
    };

    tape_init(LEN, input);
    print_tapes();

    bool accept = delta();

    if (accept) {
        printf("ACCEPT\n");
    } else {
        printf("REJECT\n");
    }

    print_tapes();
}
