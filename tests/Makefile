PROG=test
OBJDIR=.obj
CC=g++

CFLAGS = -Wall -I.. 
LDFLAGS =

$(shell mkdir -p $(OBJDIR)) 

OBJS = $(OBJDIR)/test.o $(OBJDIR)/metar.o

$(PROG) : $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(PROG)

-include $(OBJS:.o=.d)

metar.cpp: ../metar.cpp
	cp ../metar.cpp .

$(OBJDIR)/%.o: %.cpp
	$(CC) -c $(CFLAGS) $*.cpp -o $(OBJDIR)/$*.o
	$(CC) -MM $(CFLAGS) $*.cpp > $(OBJDIR)/$*.d
	@mv -f $(OBJDIR)/$*.d $(OBJDIR)/$*.d.tmp
	@sed -e 's|.*:|$(OBJDIR)/$*.o:|' < $(OBJDIR)/$*.d.tmp > $(OBJDIR)/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $(OBJDIR)/$*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $(OBJDIR)/$*.d
	@rm -f $(OBJDIR)/$*.d.tmp

clean:
	rm -rf $(PROG) $(OBJDIR) metar.cpp

