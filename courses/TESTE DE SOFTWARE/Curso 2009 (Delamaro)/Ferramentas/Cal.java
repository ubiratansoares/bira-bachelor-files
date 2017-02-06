import java.util.*;

public class Cal {

static String  dayw = "Do Se Te Qa Qi Se Sa";

static String  smon[]= new String[] {
  "Janeiro", "Fevereiro", "Março", "Abril",
  "Maio", "Junho", "Julho", "Agosto",
  "Setembro", "Outubro", "Novembro", "Dezembro",
};

static char[]  string = new char[432];
static public void main(String[] argv)
{
  int y;
  int m;
  int argc = argv.length;

  if(argc >= 2 || argc == 0)
  {
    /*
     * print out just month
     */
    if(argc == 0) {      /* current month */
      Calendar cl = Calendar.getInstance();
      m = cl.get(Calendar.MONTH) + 1;
      y = cl.get(Calendar.YEAR);
    } else {
      m = 0;
      try {
        m = Integer.parseInt(argv[0]);
      }
      catch (Exception e)
      {
        ;
      }
      if(m<1 || m>12) {
        System.err.printf("Cal: %s: mes invalido.\n", argv[0]);
        System.exit(1);
      }
      y = 0;
      try {
        y = Integer.parseInt(argv[1]);
      }
      catch (Exception e)
      {
        ;
      }
      if(y<1 || y>9999) {
        System.err.printf("Cal: %s: ano invalido.\n", argv[1]);
        System.exit(2);
      }
    }
    System.out.printf("   %s %d\n", smon[m-1], y);
    System.out.printf("%s\n", dayw);
    String s = cal(firstOfMonth(m,y),numberOfDays(m,y) );
    System.out.println(s);
  }
  else
  {
    /*
     * print out complete year
     */
    y = 0;
    try {
      y = Integer.parseInt(argv[0]);
    }
    catch (Exception e)
    {
      ;
    }
    if(y<1 || y>9999) {
      System.err.printf("Cal: %s: ano invalido.\n", argv[0]);
      System.exit(2);
    }
    for (int zi = 1; zi < 13; zi++)
    {
      System.out.printf("   %s %d\n", smon[zi-1], y);
      System.out.printf("%s\n", dayw);
      String s = cal(firstOfMonth(zi,y),numberOfDays(zi,y) );
      System.out.println(s);
      System.out.println();
    }
  }
}


static int  mon[] = {
  0,
  31, 28, 31, 30,
  31, 30, 31, 31,
  30, 31, 30, 31,
};


static int firstOfMonth(int m, int a)
{
   int k = 0;
   if (isLeap(a) && m > 2)
      k++;
   for (int i = 0; i < m; i++)
      k += mon[i];
   if ( a == 1752 && m > 9 )
      k -= 11;
   k %= 7;
   k = (k + jan1(a)) % 7;
   return k;
}

static int numberOfDays(int m, int a)
{
   if (isLeap(a) && m == 2)
      return 29;
   if ( a == 1752 && m == 9 )
      return 19;
   return mon[m];
}



static boolean isLeap(int a)
{
  if ( a <= 1752 )
  {
      if ( a % 4 == 0 )
        return true;
      else
        return false;
  }
  else
  {
      if ( a % 400 == 0 )
        return true;
      if ( a % 100 == 0 )
        return false;
      if ( a % 4 == 0 )
        return true;
  }
  return false;
}


static String cal(int dds, int n)
{
  if ( n == 19 )
      return "       1  2 14 15 16\n17 18 19 20 21 22 23\n24 25 26 27 28 29 30";
  String s = "";
  for (int k = 0; k < dds; k++)
      s += "   ";
  int cont = dds+1;
  for (int k = 1; k <= n; k++, cont++)
  {
      if ( k < 10 ) s += " ";
      s += k;
      if ( cont % 7 == 0)
          s += "\n";
      else
          s += " ";
  }
  return s;
}



/*
 *  return day of the week
 *  of jan 1 of given year
 */

static private int jan1(int yr)
{
  int y, d;

/*
 *  normal gregorian calendar
 *  one extra day per four years
 */

  y = yr;
  d = 4+y+(y+3)/4;

/*
 *  julian calendar
 *  regular gregorian
 *  less three days per 400
 */

  if(y > 1800) {
    d -= (y-1701)/100;
    d += (y-1601)/400;
  }

/*
 *  great calendar changeover instant
 */

  if(y > 1752)
    d += 3;

  return(d%7);
}

}
