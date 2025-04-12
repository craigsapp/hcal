//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Jul 13 12:32:41 PDT 1999
// Last Modified: Wed Jul 14 19:12:55 PDT 1999
// Filename:      hcal.cpp
// Syntax:        C++ 
// $Smake:        g++ -static -o %b Calendar.cpp Options.cpp %
//                Options_private.cpp %f && strip %b
//
// Description:   Historic Calendar generation.
//                Give the calendar of the specified location
//                with the correct choice made between Gregorian and
//                Julian calendars for the specified location.
//

// Notes:  3 Jan 1 Julian = 1 Jan 1 Gregorian proleptic
// 
// The beginning of the Julian year is 25 March, some regions adopted
//   1 Jan as the beginning of the year before converting to Gregorian
//   calendar.
//
// The beginning of the Gregorian year is 1 Jan without exception.
//
// A "Nicene Day" has been defined for this program.  It is defined as
// the first day in each calendar (Julian and Gregorian proleptic) which the
// day names correspond:
// The first day on which the two calendars maintain the same day is:
//        1 Mar 200 Julian = 1 Mar 200 Gregorian proleptic
// The last day on which they coincide is:
//       28 Feb 300 Julian = 28 Feb 300 Gregorian proleptic
//       29 Feb 300 Julian =  1 Mar 300 Gregorian proleptic
//

#include "Calendar.h"
#include "Options.h"
#include <cstring>
#include <iostream>
#include <cstdio>

using namespace std;

#define DISPLAY_UNKNOWN  -1
#define DISPLAY_MONTH     1
#define DISPLAY_YEAR      2
#define DISPLAY_NICENE    3
#define DISPLAY_DEBUG     4

// global variables:
Calendar cal;          // calendar object which will determine what
                       // calendar to use for the assgned dates

// global variables for command-line options:
Options options;
int   year        = YEAR_UNKNOWN;     // command-line year 
int   month       = MONTH_UNKNOWN;    // command-line year
int   day         = DAY_UNKNOWN;      // command-line year
int   displayType = DISPLAY_UNKNOWN;  // for deciding what to display
int   yearDisplayType = 0;            // for regular cal style year calendar

// function declarations:
char*         centerline      (char* buffer, const char* string, 
                                 int linelen, char rfill = '\0');
void          checkOptions    (Options& opts);
void          locales         (void);
void          example         (void);
void          help            (void);
void          usage           (const char* command);

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   options.setOptions(argc, argv);
   checkOptions(options);
   
   char buffer[128] = {0};
   int calendar = CALENDAR_UNKNOWN;
   switch (displayType) {
      case DISPLAY_MONTH:
         if (options.getBoolean("label")) {
            calendar = cal.getMonthCalendar();
            cout << centerline(buffer, 
                  Calendar::getCalendarName(calendar), 20, ' ');
            cout << '\n';
         }
         cal.printMonth();
         break;
      case DISPLAY_YEAR:
         if (options.getBoolean("label")) {
            calendar = cal.getYearCalendar();
            if (options.getBoolean("single")) {
               cout << centerline(buffer, 
                     Calendar::getCalendarName(calendar), 20, ' ');
            } else {
               cout << centerline(buffer, 
                     Calendar::getCalendarName(calendar), 66, ' ');
            }
            cout << '\n';
         }
         cal.printYear(cout, yearDisplayType);
         break;
      case DISPLAY_NICENE:
         cout << "Day number is: " << cal.getNiceneDay() << " for " 
              << Calendar::getCalendarName(
                 Calendar::getCalendar(cal.getLocale(), cal.getNiceneDay()))
              << " calendar" << endl;
              break;
      case DISPLAY_DEBUG:
         cout << "Julian Nicene day is: " 
              << Calendar::niceneDay(CALENDAR_JULIAN, year, month, day) 
              << endl;
         cout << "Gregorian Nicene day is: " 
              << Calendar::niceneDay(CALENDAR_GREGORIAN, year, month, day) 
              << endl;
         cout << "Julian Year is: "
              << Calendar::getYear(CALENDAR_JULIAN, 
                    Calendar::niceneDay(CALENDAR_JULIAN, year, month, day))
              << endl;
         cout << "Gregorian Year is: "
              << Calendar::getYear(CALENDAR_GREGORIAN, 
                    Calendar::niceneDay(CALENDAR_GREGORIAN, year, month, day))
              << endl;
         cout << "Julian Month is: "
              << Calendar::getMonth(CALENDAR_JULIAN, 
                    Calendar::niceneDay(CALENDAR_JULIAN, year, month, day))
              << endl;
         cout << "Gregorian Month is: "
              << Calendar::getMonth(CALENDAR_GREGORIAN, 
                    Calendar::niceneDay(CALENDAR_GREGORIAN, year, month, day))
              << endl;
         cout << "Julian Day is: "
              << Calendar::getDay(CALENDAR_JULIAN, 
                    Calendar::niceneDay(CALENDAR_JULIAN, year, month, day))
              << endl;
         cout << "Gregorian Day is: "
              << Calendar::getDay(CALENDAR_GREGORIAN, 
                    Calendar::niceneDay(CALENDAR_GREGORIAN, year, month, day))
              << endl;
   }

   return 0;
}

///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// centerline -- center the given string in the buffer according to the 
//     given line length default value: rfill = '\0'
//

char* centerline(char* buffer, const char* string, int linelen, char rfill) {
   int length = strlen(string);
   if (length > linelen) {
      strncpy(buffer, string, linelen);
   }

   int rightside = (linelen - length)/2;
   int leftside = linelen - length - rightside;

   int i;
   for (i=0; i<leftside; i++) {
      buffer[i] = ' ';
   }
   for (i=0; i<length; i++) {
      buffer[leftside+i] = string[i];
   }
   for (i=0; i<rightside; i++) {
      buffer[leftside+length+i] = rfill;
   }

   buffer[linelen] = '\0';
   return buffer;
}



//////////////////////////////
//
// checkOptions -- processes the command-line options
//

void checkOptions(Options& opts) {
   // calendar type options
   opts.define("gregorian|Gregorian|g=b");
   opts.define("julian|Julian|j=b");

   // locale type options
   opts.define("Rome|rome|Italy|italy|i=b");
   opts.define("France|france|f=b");
   opts.define("Bavaria|bavaria|b=b");
   opts.define("Austria|austria|Bohemia|bohemia|Slovakia|slovakia|a=b");
   opts.define("Lucerne|lucerne|Catholic-Switzerland|catholic-switzerland|l=b");
   opts.define("Hungary|hungary|h=b");
   opts.define("Norway|norway|Denmark|denmark|Danmark|danmark|n=b");
   opts.define
         ("Zurich|zurich|Protestant-Switzerland|protestant-switzerland|z=b");
   opts.define("England|england|u|UK|uk|Ireland|ireland|Scotland|scotland=b");
   opts.define("Russia|russia|r=b");
   opts.define("Romania|romania|m=b");
   opts.define("Greece|greece|Hellas|hellas|e=b");
   opts.define("Turkey|turkey|t=b");

   // other options
   opts.define("s|single-column|single=b");  // print year cal in single col.
   opts.define("nicene=b");                  // print the Nicene day if 3 args
   opts.define("label=b");                   // list type of calendar printed
   opts.define("locales=b");                 // print a list of locales
   opts.define("early=b");                   // if using a small year number

   // standard options
   opts.define("author=b");
   opts.define("version=b");
   opts.define("help=b");
   opts.define("morehelp=b");
   opts.define("example=b");
   options.process();

   int locale = LOCALE_UNKNOWN;
   // set the locale type variable:
   if (opts.getBoolean("Gregorian")) {
      locale = LOCALE_GREGORIAN;
   } else if (opts.getBoolean("Julian")) {
      locale = LOCALE_JULIAN;
   } else if (opts.getBoolean("Rome")) {
      locale = LOCALE_ROME;
   } else if (opts.getBoolean("France")) {
      locale = LOCALE_FRANCE;
   } else if (opts.getBoolean("Bavaria")) {
      locale = LOCALE_BAVARIA;
   } else if (opts.getBoolean("Austria")) {
      locale = LOCALE_AUSTRIA;
   } else if (opts.getBoolean("Lucerne")) {
      locale = LOCALE_LUCERNE;
   } else if (opts.getBoolean("Hungary")) {
      locale = LOCALE_HUNGARY;
   } else if (opts.getBoolean("Norway")) {
      locale = LOCALE_NORWAY;
   } else if (opts.getBoolean("Zurich")) {
      locale = LOCALE_ZURICH;
   } else if (opts.getBoolean("England")) {
      locale = LOCALE_ENGLAND;
   } else if (opts.getBoolean("Russia")) {
      locale = LOCALE_RUSSIA;
   } else if (opts.getBoolean("Romania")) {
      locale = LOCALE_ROMANIA;
   } else if (opts.getBoolean("Greece")) {
      locale = LOCALE_GREECE;
   } else if (opts.getBoolean("Turkey")) {
      locale = LOCALE_TURKEY;
   } else {
      locale = LOCALE_ENGLAND;
   }

   // set the calendar type variable:
   if (opts.getBoolean("Gregorian")) {
      locale = LOCALE_GREGORIAN;
   } else if (opts.getBoolean("Julian")) {
      locale = LOCALE_JULIAN;
   }


   if (opts.getBoolean("author")) {
      cout << "Written by Craig Stuart Sapp, "
              "craig@ccrma.stanford.edu, \n"
              "14 July 1999 (in the Gregorian calendar)" << endl;
      exit(0);
   } else if (opts.getBoolean("version")) {
      cout << "hcal, version 1.0 (14 July 1999)\n"
              "compiled: " << __DATE__ << endl;
      exit(0);
   } else if (opts.getBoolean("help")) {
      usage(opts.getCommand().c_str());
      exit(0);
   } else if (opts.getBoolean("morehelp")) {
      help();
      exit(0);
   } else if (opts.getBoolean("example")) {
      example();
      exit(0);
   } else if (opts.getBoolean("locales")) {
      locales();
      exit(0);
   }
   
   if (opts.getBoolean("single")) {
      yearDisplayType = 1;
   } else {
      yearDisplayType = 0;
   }

   switch (opts.getArgCount()) {
      case 1:
         std::sscanf(opts.getArg(1).c_str(), "%d", &year);
         month = 1;
         day = 1;
         displayType = DISPLAY_YEAR;
         break;
      case 2:
         std::sscanf(opts.getArg(1).c_str(), "%d", &month);
         std::sscanf(opts.getArg(2).c_str(), "%d", &year);
         day = 1;
         displayType = DISPLAY_MONTH;
         break;
      case 3:
         std::sscanf(opts.getArg(1).c_str(), "%d", &day);
         std::sscanf(opts.getArg(2).c_str(), "%d", &month);
         std::sscanf(opts.getArg(3).c_str(), "%d", &year);
         displayType = DISPLAY_NICENE;
         break;
     default: 
         usage(opts.getCommand().c_str());
         exit(1);
   }

   if (year < 1) {
      cout << "Error: year " << year << " cannot be handled by this program" 
           << endl;
      exit(1);
   }
   if (year < 100 && !opts.getBoolean("early")) {
      cout << "Error: year " << year << " is too small." << endl;
      cout << "Either specify the century, or use the --early option" << endl;
      exit(1);
   }

   cal.setDate(year, month, day, locale);
}



//////////////////////////////
//
// example -- examples of how to run the program.
//

void example(void) {
   cout << 
   "examples will go here\n"
   "\n"
   "\n"
   "\n"
   << endl;


}



//////////////////////////////
//
// help -- describe the locales, etc.
//

void help(void) {
   cout << 
   "Help goes here\n"
   "\n"
   "\n"
   "\n"
   "\n"
   << endl;
}



//////////////////////////////
//
// locales -- prints the locales used to choose the Julian or 
//     Gregorian Calendars
//

void locales(void) {
   cout <<
   "Rome:                5-14 October   1582 dropped.\n"
   "   Includes the rest of Italy, Spain, Portugal.\n"
   "\n"
   "France:             10-19 December  1582 dropped.\n"
   "\n"
   "Bavaria:             6-15 October   1583 dropped.\n"
   "\n"
   "Austria:             7-16 January   1584 dropped.\n"
   "   Includes Slovakia and Bohemia.\n"
   "\n"
   "Lucerne:            12-21 January   1584 dropped.\n"
   "\n"
   "Hungary:            22-31 October   1587 dropped.\n"
   "\n"
   "Norway:             19-28 February  1700 dropped.\n"
   "   Includes Denmark.\n"
   "\n"
   "Zurich:              1-11 February  1701 dropped.\n"
   "   The Protestant regions of Switzerland.\n"
   "\n"
   "England:             3-13 September 1752 dropped.\n"
   "   Includes Ireland and Scotland.\n"
   "\n"
   "Russia:              1-13 February  1918 dropped.\n"
   "\n"
   "Romania:             1-13 April     1919 dropped.\n"
   "\n"
   "Greece:             10-22 March     1924 dropped.\n"
   "\n"
   "Turkey:             19-31 December  1926 dropped.\n"
   "\n"
   << endl;

}


//////////////////////////////
//
// usage -- prints the synopsis of the command and possible options.
//

void usage (const char* command) {
   cout <<
   "-g   generates a Gregorian calendar for the given date.\n"
   "-j   generates a Julian calendar for the given date.\n"
   "-i   generates a calendar for Rome (Italy) [also Portugal, Spain,\n"
   "        and parts of Poland] by subtracting the days 5-14 October from \n"
   "        the year 1582.\n"
   "-f   generates a calendar for France by subtracting the days\n"
   "        10-19 December from the month of December 1582.\n"
   "-b   generates a calendar for Bavaria and other South German\n"
   "        provinces by dropping the days 6-15 October from the year 1583.\n"
   "-a   generates a calendar for Austria [also Bohemia and\n"
   "        Slovakia] by dropping the days 7-16 January from the year 1584.\n"
   "-s   generates a calendar for the Catholic cantons of\n"
   "        Switzerland [e.g., Lucerne] by dropping the days 12-21 January\n"
   "        from the year 1584.\n"
   "-h   generates a calendar for Hungary [also parts of Poland]\n"
   "        by dropping the days 22-31 October from the year 1587.\n"
   "-n   generates a calendar for Norway [also Denmark and many\n"
   "        North German provinces] by dropping the days 19-28 February\n"
   "        from the year 1700.\n"
   "-z   generates a calendar for the Protestant cantons of\n"
   "        Switzerland [e.g., Zurich] by dropping the days 1-11 January\n"
   "        from the year 1701.\n"
   "-u   generates a calendar for the United Kingdom (England, Scotland, \n"
   "        Wales, Ireland, and the British colonies by dropping the days\n"
   "        3-13 September 1752 (default option).\n"
   "-r   generates a calendar for Russia and the other provinces\n"
   "        of the former U.S.S.R. by dropping the days 1-13 February from\n"
   "        the year 1918.\n"
   "-e   generates a calendar for Greece by dropping the days\n"
   "        10-22 March 1924.\n"
   "-t   generates a calendar for Turkey by dropping the days\n"
   "        19-31 December from the year 1926.\n"
   "\n"
   << endl;
}



