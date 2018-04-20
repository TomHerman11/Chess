CC = gcc
OBJS = main.o ConsoleMain.o GuiMain.o \
ChessGame.o ChessHistory.o ChessMainAux.o ChessMinimax.o ChessParser.o \
GuiMainWindow.o GuiSettingsWindow.o GuiGameWindow.o GuiSaveLoadWindow.o \
 GuiMainAux.o highlightBoard.o PieceWidget.o Widget.o Button.o


EXEC = chess
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors
SDL_COMP_FLAG = -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT
SDL_LIB = -L/usr/local/lib/sdl_2.0.5/lib -Wl,-rpath,/usr/local/lib/sdl_2.0.5/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2main

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(SDL_LIB) -o $@
main.o: main.c ConsoleMain.h GuiMain.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ConsoleMain.o: ConsoleMain.c ConsoleMain.h ChessMainAux.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
GuiMain.o: GuiMain.c GuiMain.h GuiMainAux.h GuiMainWindow.h GuiSettingsWindow.h GuiGameWindow.h GuiSaveLoadWindow.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c


ChessGame.o: ChessGame.c ChessGame.h ChessHistory.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ChessHistory.o: ChessHistory.c ChessHistory.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ChessMainAux.o: ChessMainAux.c ChessMainAux.h ChessMinimax.h ChessParser.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ChessMinimax.o: ChessMinimax.c ChessMinimax.h ChessGame.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ChessParser.o: ChessParser.c ChessParser.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c


GuiMainWindow.o: GuiMainWindow.c GuiMainWindow.h ChessGame.h Button.h GuiMainAux.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
GuiSettingsWindow.o: GuiSettingsWindow.c GuiSettingsWindow.h ChessGame.h Button.h GuiMainAux.h ChessMainAux.h PieceWidget.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
GuiGameWindow.o: GuiGameWindow.c GuiGameWindow.h ChessGame.h Button.h GuiMainAux.h PieceWidget.h ChessMinimax.h highlightBoard.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c	
GuiSaveLoadWindow.o: GuiSaveLoadWindow.c GuiSaveLoadWindow.h ChessGame.h Button.h GuiMainAux.h ChessMainAux.h PieceWidget.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c	


GuiMainAux.o: GuiMainAux.c GuiMainAux.h PieceWidget.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
highlightBoard.o: highlightBoard.c highlightBoard.h ChessGame.h Button.h GuiGlobalArgs.h ChessMainAux.h GuiMainAux.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
PieceWidget.o: PieceWidget.c PieceWidget.h ChessGame.h GuiGlobalArgs.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
Widget.o: Widget.c Widget.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
Button.o: Button.c Button.h Widget.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c


clean:
	rm -f *.o $(EXEC) $(UNIT_TESTS)
