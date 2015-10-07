//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Jul 14 19:22:23 PDT 1999
// Last Modified: Wed Jul 14 19:22:27 PDT 1999
// Filename:      Calendar.h
// Syntax:        C++ 
//
// Description:   Historic Calendar generation.
//

#ifndef _CALENDAR_H_INCLUDED
#define _CALENDAR_H_INCLUDED

#include <iostream>

using namespace std;

#define YEAR_UNKNOWN        -999999
#define MONTH_UNKNOWN       -999999
#define DAY_UNKNOWN         -999999

// These are the types of calendars that can be handled by this class:
#define CALENDAR_UNKNOWN    -999999
#define CALENDAR_GREGORIAN  -999998
#define CALENDAR_JULIAN     +999998
#define CALENDAR_REFORMATION      0


// These are the first Nicene days on which the Gregorian calendar was adopted
// for each region.
#define LOCALE_UNKNOWN      -999999    /* Unknown local type                */
#define LOCALE_GREGORIAN    -999998    /* for Gregorian Calender all dates  */
#define LOCALE_JULIAN       +999998    /* for Julian calender all dates     */
#define LOCALE_ROME          504993    /*  5-14 October   1582 dropped      */
#define LOCALE_FRANCE        505059    /* 10-19 December  1582 dropped      */
#define LOCALE_BAVARIA       505359    /*  6-15 October   1583 dropped      */
#define LOCALE_AUSTRIA       505452    /*  6-16 January   1584 dropped      */
#define LOCALE_LUCERNE       505457    /* 12-21 January   1584 dropped      */
#define LOCALE_HUNGARY       506836    /* 22-31 October   1587 dropped      */
#define LOCALE_NORWAY        547864    /* 18-28 February  1700 dropped      */
#define LOCALE_ZURICH        548181    /*  1-11 February  1701 dropped      */
#define LOCALE_ENGLAND       567054    /*  3-13 September 1752 dropped      */
#define LOCALE_RUSSIA        627471    /*  1-13 February  1918 dropped      */
#define LOCALE_ROMANIA       627895    /*  1-13 April     1919 dropped      */
#define LOCALE_GREECE        629700    /* 10-22 March     1924 dropped      */
#define LOCALE_TURKEY        630714    /* 19-31 December  1926 dropped      */




class Calendar {
   public:
                         Calendar        (void);
                        ~Calendar        ();

      int                getCalendar     (void);
      int                getDay          (void);
      int                getLocale       (void) const;
      int                getMonth        (void);
      int                getMonthCalendar(void);
      int                getNiceneDay    (void) const;
      int                getYear         (void);
      int                getYearCalendar (void);
      ostream&           printMonth      (ostream& out = cout, 
                                            int aMonth = MONTH_UNKNOWN,
                                            int style = 0);
      ostream&           printYear       (ostream& out = cout, int ttype = 0);
      void               setDate         (int year, int month, int day, 
                                             int aLocale = LOCALE_UNKNOWN);
      void               setGregorianDate(int year, int month, int day);
      void               setJulianDate   (int year, int month, int day);
      void               setLocale       (int aLocale);
    

   // static public functions
      static int         daysInYear      (int calendar, int year);
      static int         dayOfWeek       (int calendar, int year, int month, 
                                               int day);
      static int         dayOfYear       (int calendar, int year, int month, 
                                               int day);
      static int         getCalendar     (int locale, int year, int month, 
                                               int day);
      static int         getCalendar     (int locale, int niceneDay);
      static const char* getCalendarName (int calendar);
      static int         getDay          (int aLocale, int niceneDay);
      static const char* getLocaleName   (int aLocale);
      static int         getMonth        (int aLocale, int niceneDay);
      static int         getYear         (int aLocale, int niceneDay);
      static int         leapYear        (int calendar, int year);
      static int         niceneDay       (int calendar, int year, int month, 
                                               int day);

   private:
      int                locale;         // locale for calendar determination
      int                nday;           // Nicene day

      static char*       centerline      (char* buffer, const char* string,
                                            int sz = 20, char fill = '\n');
      static void        validateMonth   (int month);
      static void        validateDay     (int day);

      // the number of days since the beginning day of the year for months 1-12:
      static const int   monthday[13];
      static const int   lmonthday[13];

};


#endif  // _CALENDAR_H_INCLUDED



