PROG=weather
OBJDIR=.obj
CC=g++

CFLAGS = -Wall --std=c++14 -I.. `pkg-config libcurl --libs`
LDFLAGS = `pkg-config libcurl --libs`

$(shell test -f metar.cpp || ln ../metar.cpp .)
$(shell test -f Curl.h || wget https://github.com/jachappell/Curlpp/raw/master/Curl.h)
$(shell test -f Fetch.h || wget https://github.com/jachappell/Curlpp/raw/master/Fetch.h)
$(shell test -f Fetch.cpp || wget https://github.com/jachappell/Curlpp/raw/master/Fetch.cpp)

$(shell mkdir -p $(OBJDIR)) 

OBJS = $(OBJDIR)/main.o $(OBJDIR)/metar.o $(OBJDIR)/Fetch.o

$(PROG) : $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(PROG)

-include $(OBJS:.o=.d)

$(OBJDIR)/%.o: %.cpp
	$(CC) -c $(CFLAGS) $*.cpp -o $(OBJDIR)/$*.o
	$(CC) -MM $(CFLAGS) $*.cpp > $(OBJDIR)/$*.d
	@mv -f $(OBJDIR)/$*.d $(OBJDIR)/$*.d.tmp
	@sed -e 's|.*:|$(OBJDIR)/$*.o:|' < $(OBJDIR)/$*.d.tmp > $(OBJDIR)/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $(OBJDIR)/$*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $(OBJDIR)/$*.d
	@rm -f $(OBJDIR)/$*.d.tmp

clean:
	rm -rf $(PROG) $(OBJDIR) metar.cpp Curl.h Fetch.h Fetch.cpp

