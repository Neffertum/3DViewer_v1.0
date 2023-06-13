CC = gcc
CFLAGS = -std=c11 -Wall -Werror -Wextra

FILES_C = ./s21_c_source/s21_moving.c ./s21_c_source/s21_parsing.c ./s21_c_source/s21_3d_viewer_1_0.h
FILES_CPP = ./s21_qt_source/mainwindow.cpp ./s21_qt_source/mainwindow.h ./s21_qt_source/mainwindow.ui ./s21_qt_source/s21_gl_view.cpp ./s21_qt_source/s21_gl_view.h ./s21_qt_source/debugwindow.cpp ./s21_qt_source/debugwindow.h ./s21_qt_source/debugwindow.ui
BUILD_FOLDER = build
P_NAME = 3d_viewer_1_0.app
T_NAME = $(P_NAME)-build-realese-v1.tar.gz
TEST_SOURSE = ./test/s21_moving.o ./test/s21_parsing.o


SOURCES = $(wildcard ./s21_c_source/s21_*.c)

OBJECTS = $(patsubst %.c, %.o, $(SOURCES))

TEST_FLAGS := -lcheck $(shell pkg-config --libs check)
OPEN_HTML := open
ifeq ($(shell uname), Linux)
	TEST_FLAGS = -lcheck -lm -lpthread -lsubunit -lrt -fprofile-arcs $(shell pkg-config --cflags --libs check)
	OPEN_HTML := xdg-open
endif
all: clean gcov_report install dvi dist
.PHONY : all

install: uninstall ./s21_qt_source/main.cpp $(FILES_C) $(FILES_CPP)
	if [ ! -f "build/$(P_NAME)" ]; then \
		cd s21_qt_source/ && qmake6 3d_viewer_1_0.pro && make; \
		if ! [ -d "./../build" ]; then \
			mkdir ./../build; \
		fi; \
		mv $(P_NAME) ./../build/; \
	fi
.PHONY : install

dvi:
	cd doxygen && doxygen Doxyfile
	$(OPEN_HTML) doxygen/html/index.html 2> /dev/null
.PHONY : dvi

dist:
	make clean
	tar --totals -cvf 3d_viewer_1_0.tar s21_c_source s21_qt_source tests Makefile examples_obj doxygen
.PHONY : dist

s21_3d_viewer_1_0.a: $(OBJECTS)
	ar rcs s21_3d_viewer_1_0.a $(OBJECTS)
	ranlib s21_3d_viewer_1_0.a
.PHONY : s21_3d_viewer_1_0.a

test: clean_files s21_3d_viewer_1_0.a
	$(CC) $(CFLAGS) tests/*.c s21_3d_viewer_1_0.a $(TEST_FLAGS) -o test
	./test
.PHONY : tests

gcov_flag:
	$(eval CFLAGS += --coverage)

gcov_report: clean gcov_flag test
	mkdir -p gcov
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory gcov
	rm -rf *.gcno *.gcda *.gcov
	$(OPEN_HTML) ./gcov/index.html
.PHONY : gcov_report

style:
	clang-format -style=google -n ./s21_c_source/*.c* ./s21_c_source/*.h* ./s21_qt_source/*.c* ./s21_qt_source/*.h*
.PHONY : style

rebuild: uninstall install
.PHONY : rebuild

clean: uninstall clean_files
.PHONY : clean

clean_files:
	rm -rf *.a *.o tests/*.o test *.a *.gcno *.gcda *.gcov *.info *.tar
	rm -rf ./*/*.o ./*/*.a ./*/*.gcno ./*/*.gcda ./*/*.gcov ./*/*.info
	rm -rf ./doxygen/html
	rm -rf *.dSYM
	rm -rf ./test
	rm -rf my_test
	rm -rf *.gcov gcov
	rm -rf coverage*
.PHONY : clean_files
		

uninstall:
	rm -rf ./s21_qt_source/$(P_NAME)
	rm -rf $(BUILD_FOLDER)
	rm -rf ./s21_qt_source/3d_viewer_1_0.pro.user
	rm -rf ./s21_qt_source/.qmake.stash
	rm -rf ./s21_qt_source/*.o
	rm -rf ./s21_qt_source/Makefile
	rm -rf ./s21_qt_source/moc_*
	rm -rf ./s21_qt_source/ui_mainwindow.h
	rm -rf ./s21_qt_source/ui_debugwindow.h
	rm -rf *.gz
.PHONY : uninstall

gcov_report_l: clean gcov_flag test
	gcov -b -l -p -c ./s21*.gcno ./s21_c_source/*.gcno
	gcovr -g -k -r . --html --html-details -o ./coverage_report.html
	$(OPEN_HTML) ./coverage_report.html
