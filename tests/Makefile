PROG1=metar_test
PROG2=conv_test
PROG3=utils_test
PROG4=cloud_test
PROG5=phenom_test
OBJDIR=.obj
CC=g++

CFLAGS = -Wall -I../include --std=c++17
LDFLAGS = -L../lib -lMetar

all: $(PROG1) $(PROG2) $(PROG3) $(PROG4) $(PROG5)

$(shell mkdir -p $(OBJDIR)) 

OBJS1 = $(OBJDIR)/metar_test.o
OBJS2 = $(OBJDIR)/conv_test.o
OBJS3 = $(OBJDIR)/utils_test.o
OBJS4 = $(OBJDIR)/cloud_test.o
OBJS5 = $(OBJDIR)/phenom_test.o

$(PROG1) : $(OBJS1) ../lib/libMetar.a
	$(CC) $(OBJS1) $(LDFLAGS) -o $(PROG1)

$(PROG2) : $(OBJS2)
	$(CC) $(OBJS2) $(LDFLAGS) -o $(PROG2)

$(PROG3) : $(OBJS3)
	$(CC) $(OBJS3) $(LDFLAGS) -o $(PROG3)

$(PROG4) : $(OBJS4) ../lib/libMetar.a
	$(CC) $(OBJS4) $(LDFLAGS) -o $(PROG4)

$(PROG5) : $(OBJS5) ../lib/libMetar.a
	$(CC) $(OBJS5) $(LDFLAGS) -o $(PROG5)

-include $(OBJS1:.o=.d)
-include $(OBJS2:.o=.d)
-include $(OBJS3:.o=.d)
-include $(OBJS4:.o=.d)
-include $(OBJS5:.o=.d)

$(OBJDIR)/%.o: %.cpp
	$(CC) -c $(CFLAGS) $*.cpp -o $(OBJDIR)/$*.o
	$(CC) -MM $(CFLAGS) $*.cpp > $(OBJDIR)/$*.d
	@mv -f $(OBJDIR)/$*.d $(OBJDIR)/$*.d.tmp
	@sed -e 's|.*:|$(OBJDIR)/$*.o:|' < $(OBJDIR)/$*.d.tmp > $(OBJDIR)/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $(OBJDIR)/$*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $(OBJDIR)/$*.d
	@rm -f $(OBJDIR)/$*.d.tmp

clean:
	rm -rf $(PROG1) $(PROG2) $(PROG3) $(PROG4) $(PROG5) $(OBJDIR)
