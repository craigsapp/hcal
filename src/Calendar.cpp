//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Jul 14 19:22:23 PDT 1999
// Last Modified: Sat Apr 12 02:35:20 PM PDT 2025 Update to use strncpy.
// Filename:      Calendar.cpp
// Syntax:        C++ 
//
// Description:   Historic Calendar generation.
//
// Note:  3 Jan 1 Julian (Monday) = 1 Jan 1 (Monday) Gregorian proleptic
// 
// The beginning of the Julian year is 25 March, some regions adopted
//   1 Jan as the beginning of the year before converting to Gregorian
//   calendar.
//
// The beginning of the Gregorian year is 1 Jan without exception.
//
// A "Nicene Day" has been defined for this program.  It is defined as
// the first day in each calendar (Julian and Gregorian proleptic) which the
// day names and dates are the same:
// The first day on which the two calendars maintain the same day is:
//        1 Mar 200 Julian = 1 Mar 200 Gregorian proleptic
// The last day on which they coincide is:
//       28 Feb 300 Julian = 28 Feb 300 Gregorian proleptic
//       29 Feb 300 Julian =  1 Mar 300 Gregorian proleptic
//


#include "Calendar.h"
#include <cstring>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <string.h>

using namespace std;

// declaration of static variables:
const int Calendar::monthday[13] = {
      0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
const int Calendar::lmonthday[13] = {
      0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};


//////////////////////////////
//
// Calendar::Calendar --
//

Calendar::Calendar(void) { 
   locale = LOCALE_UNKNOWN;
   nday   = DAY_UNKNOWN;
}



//////////////////////////////
//
// Calendar::~Calendar --
//

Calendar::~Calendar() { }



//////////////////////////////
//
// Calendar::daysInYear -- return the number of days in the year of 
//   the given calendar.
//

int Calendar::daysInYear(int calendar, int year) {
   if (leapYear(calendar, year)) {
      return 366;
   } else {
      return 365;
   }
}



//////////////////////////////
//
// Calendar::dayOfWeek -- returns the day of the week, where
//   0 = Sunday, 1=Monday, 2=Tuesday, 3=Wednesday, etc.
//

int Calendar::dayOfWeek(int calendar, int year, int month, int day) {
   return ((niceneDay(calendar, year, month, day) % 7 - 1) + 14) % 7;
}



//////////////////////////////
//
// Calendar::dayOfYear -- returns the day number of the date, with
//     1 Jan being day 0.
//

int Calendar::dayOfYear(int calendar, int year, int month, int day) {
   validateMonth(month);
   validateDay(day);
 
   if (leapYear(calendar, year)) {
      return lmonthday[month] + (day-1);
   } else { 
      return monthday[month] + (day-1);
   }
}



//////////////////////////////
//
// Calendar::getCalendar -- returns the calendar associated with a specific
//     locale.
//

int Calendar::getCalendar(int aLocale, int year, int month, int day) {
   if (aLocale == LOCALE_UNKNOWN) {
      return CALENDAR_UNKNOWN;
   }

   int aNiceneDay = niceneDay(CALENDAR_GREGORIAN, year, month, day);

   return getCalendar(aLocale, aNiceneDay);
}


int Calendar::getCalendar(int aLocale, int aNiceneDay) {
   if (aNiceneDay >= aLocale) {
      return CALENDAR_GREGORIAN;
   } else {
      return CALENDAR_JULIAN;
   }
}


int Calendar::getCalendar(void) {
   return getCalendar(getLocale(), getNiceneDay());
}



//////////////////////////////
//
// Calendar::getCalendarName -- returns the calendar name of the given
//     calendar code.
//

const char* Calendar::getCalendarName(int calendar) {
   switch (calendar) {
      case CALENDAR_JULIAN:
         return "Julian";
      case CALENDAR_GREGORIAN:
         return "Gregorian";
      case CALENDAR_REFORMATION:
         return "Reform Year";
      case CALENDAR_UNKNOWN:
      default:
         return "Unknown";
   }
}



//////////////////////////////
//
// Calendar::getDay -- return the day of the month.
//

int Calendar::getDay(int aLocale, int aNiceneDay) {
   int calendar = getCalendar(aLocale, aNiceneDay);
   int year = getYear(aLocale, aNiceneDay);
   
   int yearNicene = niceneDay(calendar, year, 1, 1);

   int dayofyear = aNiceneDay - yearNicene;

   
   const int* montharray;
   if (leapYear(calendar, year)) {
      montharray = lmonthday;
   } else {
      montharray = monthday;
   }

   int month = 1;
   while (month < 13 && montharray[month] <= dayofyear) {
      month++;
   }
   month--;
   if (month == 0) {
      month = 1;
   }
   if (month >= 13) {
      month = 12;
   }
 
   if (month < 1 || month > 12) {
      cout << "Error in getDay, month is: " << month << endl;
   }

   return dayofyear - montharray[month] + 1;
}


int Calendar::getDay(void) {
   return getDay(getLocale(), getNiceneDay());
}



//////////////////////////////
//
// Calendar::getLocale --
//

int Calendar::getLocale(void) const { 
   return locale;
}



//////////////////////////////
//
// Calendar::getLocaleName -- returns the locale name of the given
//     locale code.
//

const char* Calendar::getLocaleName(int aLocale) {
   switch (aLocale) {
      case LOCALE_ROME:
         return "Rome";
      case LOCALE_FRANCE:
         return "France";
      case LOCALE_ENGLAND:
         return "England";
      case LOCALE_UNKNOWN:
      default:
         return "Unknown";
   }
}



//////////////////////////////
//
// Calendar::getMonth -- offset from 1 for January, 12=december
//

int Calendar::getMonth(int aLocale, int aNiceneDay) {
   int calendar = getCalendar(aLocale, aNiceneDay);
   int year = getYear(aLocale, aNiceneDay);
   
   int yearNicene = niceneDay(calendar, year, 1, 1);

   int dayofyear = aNiceneDay - yearNicene;

   
   const int* montharray;
   if (leapYear(calendar, year)) {
      montharray = lmonthday;
   } else {
      montharray = monthday;
   }

   int month = 1;
   while (month < 13 && montharray[month] <= dayofyear) {
      month++;
   }
   month--;
   if (month == 0) {
      month = 1;
   }
   if (month >= 13) {
      month = 12;
   }

   if (month < 1 || month > 12) {
      cout << "Error in getMonth() function: " << month << endl;
      exit(1);
   } else {
      return month;
   }
}


int Calendar::getMonth(void) {
   return getMonth(getLocale(), getNiceneDay());
}



//////////////////////////////
//
// Calendar::getMonthCalendar -- returns the calendar used for
//    the given local of the month in question.
//

int Calendar::getMonthCalendar(void) {
   int year = getYear();
   int month = getMonth();

   // handle special case for Protestand Switzerland where Jan 1, 1701 does not exist:
   if (getLocale() == LOCALE_ZURICH && year == 1701 && month == 1) {
      return CALENDAR_REFORMATION;
   } else if (getLocale() == LOCALE_ZURICH && year == 1700 && month == 12) {
      return CALENDAR_JULIAN;
   }
  
   Calendar cal1;
   Calendar cal2;

   cal1.setDate(year, month, 1, getLocale());
   if (month == 12) {
      cal2.setDate(year+1, 1, 1, getLocale());
   } else {
      cal2.setDate(year, month+1, 1, getLocale());
   }
   
   int calendar1 = cal1.getCalendar();
   int calendar2 = cal2.getCalendar();

   if (calendar1 == calendar2) {
      return calendar1;
   } else {
      return CALENDAR_REFORMATION;
   }
}



//////////////////////////////
//
// Calendar::getNiceneDay -- 
//

int Calendar::getNiceneDay(void) const {
   return nday;
}



//////////////////////////////
//
// Calendar::getYear -- returns the year of the specified nday.
//

int Calendar::getYear(int aLocale, int aNiceneDay) {
   int calendar = getCalendar(aLocale, aNiceneDay);

   switch (calendar) {
      case CALENDAR_JULIAN:
         return (int)((aNiceneDay+72745)/365.25+1);
         break;
      case CALENDAR_GREGORIAN:
         return (int)((aNiceneDay+72743)/365.2425+1);
         break;
      default:
         cout << "Error: unknown calendar type:" << calendar << endl;
         exit(1);
   }
}


int Calendar::getYear() {
   return getYear(getLocale(), getNiceneDay());
}



//////////////////////////////
//
// Calendar::getYearCalendar -- returns the calendar type for the current year.
//

int Calendar::getYearCalendar(void) {
   int year = getYear();

   // handle special case for Protestand Switzerland where Jan 1, 1701 does not exist:
   if (getLocale() == LOCALE_ZURICH && year == 1701) {
      return CALENDAR_REFORMATION;
   } else if (getLocale() == LOCALE_ZURICH && year == 1700) {
      return CALENDAR_JULIAN;
   }

   Calendar cal1;
   Calendar cal2;

   cal1.setDate(year, 1, 1, getLocale());
   cal2.setDate(year+1, 1, 1, getLocale());

   int calendar1 = cal1.getCalendar();
   int calendar2 = cal2.getCalendar();

   if (calendar1 == calendar2) {
      return calendar1;
   } else {
      return CALENDAR_REFORMATION;
   }
}



//////////////////////////////
//
// Calendar::leapYear -- return true if the year is a leap year for the 
//   the given calendar.
//

int Calendar::leapYear(int calendar, int year) {
   switch (calendar) {
      case CALENDAR_JULIAN:
         if (year % 4 == 0) {
            return 1;
         } else {
            return 0;
         }
         break;
      case CALENDAR_GREGORIAN:
         if (year % 400 == 0) {
            return 1;
         } else if (year % 100 == 0) {
            return 0;
         } else if (year % 4 == 0) {
            return 1;
         } else {
            return 0;
         }
         break;
      default:
         cout << "Error: unknown calendar type." << endl;
         exit(1);
   }
}



//////////////////////////////
//
// Calendar::niceneDay -- represents the number of days since
//   1 March 200.  1 March 200 is day 0.
//

int Calendar::niceneDay(int calendar, int year, int month, int day) {
   validateMonth(month);
   validateDay(day);

   switch (calendar) {
      case CALENDAR_JULIAN:
         return (year-1)*365 + (year-1)/4 
              + dayOfYear(calendar, year, month, day)
              - 72744 /* days since 1 Mar 200 */;
         break;
      case CALENDAR_GREGORIAN:
         return (year-1)*365 + (year-1)/4 
              - (year-1)/100 + (year-1)/400
              + dayOfYear(calendar, year, month, day)
              - 72742 /* days since 1 Mar 200 */;
         break;
      default:
         cout << "Error: unknown calendar style: " << calendar << endl;
         exit(1);
   }
}



//////////////////////////////
//
// Calendar::printYear -- print out the current year's month
//     default value: out = cout, int type = 0
//

ostream& Calendar::printYear(ostream& out, int ttype) { 
   stringstream s_month[13];
   char str_jan[8 * 21] = {0};
   char str_feb[8 * 21] = {0};
   char str_mar[8 * 21] = {0};
   char str_apr[8 * 21] = {0};
   char str_may[8 * 21] = {0};
   char str_jun[8 * 21] = {0};
   char str_jul[8 * 21] = {0};
   char str_aug[8 * 21] = {0};
   char str_sep[8 * 21] = {0};
   char str_oct[8 * 21] = {0};
   char str_nov[8 * 21] = {0};
   char str_dec[8 * 21] = {0};

   char buf[128] = {0};
   char yearbuf[32] = {0};
   int year = getYear();
   sprintf(yearbuf, "%d", year);

   int i;
   int length = 0;
   if (ttype == 1) {
      for (i=1; i<=12; i++) {
         printMonth(s_month[i], i, 10);
         s_month[i] << ends;
      }
      out << centerline(buf, yearbuf, 20, ' ') << '\n';
      for (i=1; i<=12; i++) {
         strncpy(str_jan, s_month[i].str().c_str(), 8 * 21);
         int q;
         for (q=0; q<=length; q++) {
            if (str_jan[q] == '\n') {
               str_jan[q] = ' ';
            }
         }
         length = strlen(str_jan);
         if (str_jan[length-2] == '\n') {
            str_jan[length-2] = ' ';
         }
         for (q=length-1; q<8*21; q++) {
            str_jan[q] = ' ';
         }
         for (q=0; q<8; q++) {
            str_jan[q*21+20] = '\n';
         }
         str_jan[8*21-1] = '\0';
         out << str_jan << '\n';
      }
   } else {
      for (i=1; i<=12; i++) {
         printMonth(s_month[i], i, 1);
         s_month[i] << ends;
      }
      strncpy(str_jan, s_month[1].str().c_str(), 8 * 21);
      strncpy(str_feb, s_month[2].str().c_str(), 8 * 21);
      strncpy(str_mar, s_month[3].str().c_str(), 8 * 21);
      strncpy(str_apr, s_month[4].str().c_str(), 8 * 21);
      strncpy(str_may, s_month[5].str().c_str(), 8 * 21);
      strncpy(str_jun, s_month[6].str().c_str(), 8 * 21);
      strncpy(str_jul, s_month[7].str().c_str(), 8 * 21);
      strncpy(str_aug, s_month[8].str().c_str(), 8 * 21);
      strncpy(str_sep, s_month[9].str().c_str(), 8 * 21);
      strncpy(str_oct, s_month[10].str().c_str(), 8 * 21);
      strncpy(str_nov, s_month[11].str().c_str(), 8 * 21);
      strncpy(str_dec, s_month[12].str().c_str(), 8 * 21);

      char* string[12];
      string[0] = str_jan;
      string[1] = str_feb;
      string[2] = str_mar;
      string[3] = str_apr;
      string[4] = str_may;
      string[5] = str_jun;
      string[6] = str_jul;
      string[7] = str_aug;
      string[8] = str_sep;
      string[9] = str_oct;
      string[10] = str_nov;
      string[11] = str_dec;

      int j = 0;
      for (int k=0; k<12; k++) {
         j = 0;
         while (string[k][j] != '\0') {
            if (string[k][j] == '\n') {
               string[k][j] = '\0';
            }
            j++;
         }
      }

      out << centerline(buf, yearbuf, 66, ' ') << '\n';
      out << centerline(buf, "", 66, ' ') << '\n';

      int length;
      int n;
      for (int m=0; m<4; m++) {
         for (j=0; j<8; j++) {
            out << &string[m*3 + 0][j * 21] << "   ";
            length = strlen(&string[m*3 + 0][j*21]);
            if (length != 20) {
               for (n=length; n<20; n++) {
                  out << ' ';
               }
            }
            out << &string[m*3 + 1][j * 21] << "   ";
            length = strlen(&string[m*3 + 1][j*21]);
            if (length != 20) {
               for (n=length; n<20; n++) {
                  out << ' ';
               }
            }
            out << &string[m*3 + 2][j * 21];
            length = strlen(&string[m*3 + 2][j*21]);
            if (length != 20) {
               for (n=length; n<20; n++) {
                  out << ' ';
               }
            }
            out << '\n';
         }
      }

   }

   return out;
}



//////////////////////////////
//
// Calendar::printMonth -- print out the current month
//   default values: out = cout, aMonth = MONTH_UNKNOWN, style = 0
//      
//

ostream& Calendar::printMonth(ostream& out, int aMonth, int style) {
   int year = getYear();
   int month;
   if (aMonth == MONTH_UNKNOWN) {
      month = getMonth();
   } else {
      month = aMonth;
   }

   char buf[32] = {0};
   char mstring[32] = {0};
   int dcount = 0;            // number of days in the particular month
   switch (month) {
      case 1:  
         if (style == 0) {
            sprintf(mstring, "January %d", year);
         } else {
            sprintf(mstring, "January");
         }
         dcount = 31;  
         break;
      case 2:  
         if (style == 0) {
            sprintf(mstring, "February %d", year);
         } else {
            sprintf(mstring, "February");
         }
         dcount = 28;  
         break;
      case 3:  
         if (style == 0) {
            sprintf(mstring, "March %d", year);
         } else {
            sprintf(mstring, "March");
         }
         dcount = 31;  
         break;
      case 4:  
         if (style == 0) {
            sprintf(mstring, "April %d", year);
         } else {
            sprintf(mstring, "April");
         }
         dcount = 30;  
         break;
      case 5:  
         if (style == 0) {
            sprintf(mstring, "May %d", year);
         } else {
            sprintf(mstring, "May");
         }
         dcount = 31;  
         break;
      case 6:  
         if (style == 0) {
            sprintf(mstring, "June %d", year);
         } else {
            sprintf(mstring, "June");
         }
         dcount = 30;  
         break;
      case 7:  
         if (style == 0) {
            sprintf(mstring, "July %d", year);
         } else {
            sprintf(mstring, "July");
         }
         dcount = 31;  
         break;
      case 8:  
         if (style == 0) {
            sprintf(mstring, "August %d", year);
         } else {
            sprintf(mstring, "August");
         }
         dcount = 31;  
         break;
      case 9:  
         if (style == 0) {
            sprintf(mstring, "September %d", year);
         } else {
            sprintf(mstring, "September");
         }
         dcount = 30;  
         break;
      case 10: 
         if (style == 0) {
            sprintf(mstring, "October %d", year);
         } else {
            sprintf(mstring, "October");
         }
         dcount = 31;  
         break;
      case 11: 
         if (style == 0) {
            sprintf(mstring, "November %d", year);
         } else {
            sprintf(mstring, "November");
         }
         dcount = 30;  
         break;
      case 12: 
         if (style == 0) {
            sprintf(mstring, "December %d", year);
         } else {
            sprintf(mstring, "December");
         }
         dcount = 31;  
         break;
      default: 
         if (style == 0) {
            sprintf(mstring, "UNKNOWN %d", year);
         } else {
            sprintf(mstring, "UNKNOWN");
         }
         dcount = 0;   
         break;
   }

   if (style != 1) {
      out << centerline(buf, mstring, 20, ' ') << '\n';  
   } else {
      out << centerline(buf, mstring) << '\n';  
   }
     

   out << "Su Mo Tu We Th Fr Sa" << endl;

   int calendar = getCalendar(getLocale(), year, month, 1);
   if (month == 2 && leapYear(calendar, year)) {
      dcount = 29;
   }

   int dayofweek = dayOfWeek(calendar, year, month, 1);

   if (dayofweek > 0) {
      out << "  ";
   }
   for (int i=1; i<dayofweek; i++) {
      out << "   ";
   }

   int lines = 0;
   int displayColumn = dayofweek;
   int daze = niceneDay(calendar, year, month, 1);
   int counter = 0;
   int onecount = 0;

   int currentDay = getDay(getLocale(), daze);
   int quitflag = 0;
   while (counter < 33 && !quitflag) {
      counter++;

      currentDay = getDay(getLocale(), daze);
      if (displayColumn == 0) {
         out.width(2);
      } else {
         out.width(3);
      }
      if (currentDay == 0) {
         currentDay = dcount;
      }
      if (currentDay == 1) {  // this needed for boundary transitions
         onecount++;
      }
      if (onecount < 2 && currentDay <= dcount) {
         out << currentDay;
      }
      if (currentDay >= dcount) {
         quitflag = 1;
      }
      daze++;

      if (displayColumn >= 6) {
         out << endl;
         lines++;
         displayColumn = 0;
      } else {
         displayColumn++;
      }
   }
   for (int j=lines; j<6; j++) {
      out << endl;
   }
      
   return out;
}



//////////////////////////////
//
// Calendar::setDate -- sets the date according to the current locale.
//     if no local is given.
//     default value: locale = LOCALE_UNKNOWN
//

void Calendar::setDate(int year, int month, int day, int aLocale) { 
   int calendar;
   if (aLocale == LOCALE_UNKNOWN) {
      if (getLocale() == LOCALE_UNKNOWN) {
         cout << "Error: unknown locale type" << endl;
         exit(1);
      }
      calendar = getCalendar(getLocale(), year, month, day);
   } else {
      calendar = getCalendar(aLocale, year, month, day);
   }

   locale = aLocale;
   nday = niceneDay(calendar, year, month, day);
}



//////////////////////////////
//
// Calendar::setGregorianDate -- sets the date according to the
//    Gregorian calendar regardless of the current locale.
//

void Calendar::setGregorianDate(int year, int month, int day) { 
   if (getLocale() == LOCALE_UNKNOWN) {
      setLocale(LOCALE_GREGORIAN);
   }
   nday = niceneDay(CALENDAR_GREGORIAN, year, month, day);
}



//////////////////////////////
//
// Calendar::setJulianDate -- sets the date according to the Julian
//      calendar regardless of the current locale.
//

void Calendar::setJulianDate(int year, int month, int day) { 
   if (getLocale() == LOCALE_UNKNOWN) {
      setLocale(LOCALE_JULIAN);
   }
   nday = niceneDay(CALENDAR_JULIAN, year, month, day);
}



//////////////////////////////
//
// Calendar::setLocale -- set the locale of the object.
//

void Calendar::setLocale(int aLocale) { 
   locale = aLocale;
}


///////////////////////////////////////////////////////////////////////////
//
// private functions
//

//////////////////////////////
//
// Calendar::centerline -- prints text in 20 columns
//     default value sz = 20; fill = '\n';
//

char* Calendar::centerline(char* buffer, const char* string, 
      int sz, char fill) {
   int len = strlen(string);
   int rightside = (sz - len)/2;
   int leftside = sz - len - rightside;
   int i;
   for (i=0; i<leftside; i++) {
      buffer[i] = ' ';
   }
   for (i=0; i<len; i++) {
      buffer[i+leftside] = string[i];
   }
   for (i=0; i<rightside; i++) {
      buffer[len+leftside+i] = fill;
   }

   buffer[leftside + len + rightside] = '\0';

   return buffer;
}



//////////////////////////////
//
// Calendar::validateMonth -- limits month range between 1 and 12.
//

void Calendar::validateMonth (int month) {
   if (month < 1 || month > 12) {
      cout << "Error: month is invalid: " << month << endl;
      exit(1);
   }
}
  


//////////////////////////////
//
// Calendar::validateDay -- limits day range between 1 and 31.
//     (not sophisticated by using months).
//

void Calendar::validateDay(int day) {
   if (day < 1 || day > 31) {
      cout << "Error: day is invalid: " << day << endl;
      exit(1);
   }
}



