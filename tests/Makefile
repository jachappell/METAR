PROG1=metar_test
PROG2=conv_test
OBJDIR=.obj
CC=g++

CFLAGS = -Wall -I../include 
LDFLAGS = -L../lib -lMetar

all: $(PROG1) $(PROG2)

$(shell mkdir -p $(OBJDIR)) 

OBJS1 = $(OBJDIR)/metar_test.o
OBJS2 = $(OBJDIR)/conv_test.o

$(PROG1) : $(OBJS1)
	$(CC) $(OBJS1) $(LDFLAGS) -o $(PROG1)

$(PROG2) : $(OBJS2)
	$(CC) $(OBJS2) $(LDFLAGS) -o $(PROG2)

-include $(OBJS1:.o=.d)
-include $(OBJS2:.o=.d)

$(OBJDIR)/%.o: %.cpp
	$(CC) -c $(CFLAGS) $*.cpp -o $(OBJDIR)/$*.o
	$(CC) -MM $(CFLAGS) $*.cpp > $(OBJDIR)/$*.d
	@mv -f $(OBJDIR)/$*.d $(OBJDIR)/$*.d.tmp
	@sed -e 's|.*:|$(OBJDIR)/$*.o:|' < $(OBJDIR)/$*.d.tmp > $(OBJDIR)/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $(OBJDIR)/$*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $(OBJDIR)/$*.d
	@rm -f $(OBJDIR)/$*.d.tmp

clean:
	rm -rf $(PROG1) $(PROG2) $(OBJDIR)
