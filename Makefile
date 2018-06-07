LIBDIR=lib
LIB=$(LIBDIR)/libMetar.a
OBJDIR=.obj
CC=g++
AR=ar

CFLAGS = -Wall -I include

$(shell mkdir -p $(LIBDIR)) 
$(shell mkdir -p $(OBJDIR)) 

OBJS = $(OBJDIR)/Metar.o $(OBJDIR)/Phenom.o $(OBJDIR)/Utils.o

$(LIB) : $(OBJS)
	$(AR) r $(LIB) $(OBJS) 

-include $(OBJS:.o=.d)

$(OBJDIR)/%.o: ./src/%.cpp
	$(CC) -c $(CFLAGS) src/$*.cpp -o $(OBJDIR)/$*.o
	$(CC) -MM $(CFLAGS) src/$*.cpp > $(OBJDIR)/$*.d
	@mv -f $(OBJDIR)/$*.d $(OBJDIR)/$*.d.tmp
	@sed -e 's|.*:|$(OBJDIR)/$*.o:|' < $(OBJDIR)/$*.d.tmp > $(OBJDIR)/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $(OBJDIR)/$*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $(OBJDIR)/$*.d
	@rm -f $(OBJDIR)/$*.d.tmp

clean:
	rm -rf $(LIBDIR) $(OBJDIR) 

