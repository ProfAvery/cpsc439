#include <stdio.h>
#include <stdbool.h>        // Requires C99 or later
#include <assert.h>

#define K 1
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

/* Transition function from p. 142 */

bool delta(void)
{
Q0:
    // q0 a -> qa BLANK R
    if (TAPE(0) == A) {
        write_tape(SYMBOLS{BLANK});
        move_head(DIRECTIONS{R});
        goto QA;
    }

    // q0 b -> qb BLANK R
    if (TAPE(0) == B) {
        write_tape(SYMBOLS{BLANK});
        move_head(DIRECTIONS{R});
        goto QB;
    }

    // q0 BLANK -> qaccept
    if (TAPE(0) == BLANK) {
        goto QACCEPT;
    }

QA:
    // qa a -> qa a R
    if (TAPE(0) == A) {
        write_tape(SYMBOLS{A});
        move_head(DIRECTIONS{R});
        goto QA;
    }

    // qa b -> qa b R
    if (TAPE(0) == B) {
        write_tape(SYMBOLS{B});
        move_head(DIRECTIONS{R});
        goto QA;
    }

    // qa BLANK -> q'a BLANK L
    if (TAPE(0) == BLANK) {
        write_tape(SYMBOLS{BLANK});
        move_head(DIRECTIONS{L});
        goto Q_PRIME_A;
    }

QB:
    // qb a -> qb a R
    if (TAPE(0) == A) {
        write_tape(SYMBOLS{A});
        move_head(DIRECTIONS{R});
        goto QB;
    }

    // qb b -> qb b R
    if (TAPE(0) == B) {
        write_tape(SYMBOLS{B});
        move_head(DIRECTIONS{R});
        goto QB;
    }

    // qb BLANK -> q'b BLANK L
    if (TAPE(0) == BLANK) {
        write_tape(SYMBOLS{BLANK});
        move_head(DIRECTIONS{L});
        goto Q_PRIME_B;
    }

Q_PRIME_A:
    // q'a a -> qL BLANK L
    if (TAPE(0) == A) {
        write_tape(SYMBOLS{BLANK});
        move_head(DIRECTIONS{L});
        goto QL;
    }

    // q'a b -> qreject
    if (TAPE(0) == B) {
        goto QREJECT;
    }

    // q'a BLANK -> qaccept
    if (TAPE(0) == BLANK) {
        goto QACCEPT;
    }

Q_PRIME_B:
    // q'b a -> qreject
    if (TAPE(0) == A) {
        goto QREJECT;
    }

    // q'b b -> qL BLANK L
    if (TAPE(0) == B) {
        write_tape(SYMBOLS{BLANK});
        move_head(DIRECTIONS{L});
        goto QL;
    }

    // q'b BLANK -> qaccept
    if (TAPE(0) == BLANK) {
        goto QACCEPT;
    }

QL:
    // qL a -> qL a L
    if (TAPE(0) == A) {
        write_tape(SYMBOLS{A});
        move_head(DIRECTIONS{L});
        goto QL;
    }

    // qL b -> qL b L
    if (TAPE(0) == B) {
        write_tape(SYMBOLS{B});
        move_head(DIRECTIONS{L});
        goto QL;
    }

    // qL BLANK -> q0 BLANK R
    if (TAPE(0) == BLANK) {
        write_tape(SYMBOLS{BLANK});
        move_head(DIRECTIONS{R});
        goto Q0;
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
