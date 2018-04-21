
# code details

EXE_DIR = .
EXE = $(EXE_DIR)/chatbot

SRC= UX.c Offended.c Backchat.c

# generic build details

CC=      gcc
COPT=    -g -std=c99
LIBS= -lm

# compile to  object code

OBJ= $(SRC:.c=.o)

.c.o:
	$(CC) $(COPT) -c -o $@ $<

# build executable

$(EXE): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) $(LIBS) -o $(EXE)

# clean up compilation

clean:
	rm -f $(OBJ) $(EXE)
