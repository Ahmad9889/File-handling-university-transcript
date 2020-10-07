#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#define SIZE 30

float credit_earned;// CREDIT EARNED FOR EACH SUBJECT
float credit_total = 0;//THE TOTAL OF CREDIT EARNED
float credit_hour = 0;//THE TOTAL OF CREDIT HOUR FOR EACH STUDENT
float GBA;

/*
    I SEPARATED THE CODE WORK FOR THREE PIECES :
    1 - THE HEADER: THE PART THAT INCLUDES THE NAME/SURNAME/STDNO.ETC.
    2 - THE DETAILS: THA PART THAT INCLUDES COURSE TITLES/CODES THE CREDIT.ETC...
    3 - THE CONCLUSION...
*/

void credit (FILE * ptrancsript, FILE * pperformances, FILE * pcriteria, FILE * pstudents,
             FILE * pregistrations, char coursecode01[], char stdno01[]);
void grade (FILE * ptrancsript, FILE * pperformances, char stdno01[],char courseCode002[],
           float lab, float mid_term, float final_term, float credit_amount);


int main ()
{
    // FILE POINTERS ...
    FILE* pcriteria = fopen ("criteria.txt","r");
	FILE* pcourse = fopen("courses.txt", "r");
	FILE* pperformances = fopen ("performances.txt", "r");
	FILE* pregistrations = fopen ("registrations.txt", "r");
	FILE* pstudents = fopen("students.txt", "r");
    FILE* ptranscript = fopen ("Transcript.txt","w");

    char stdno[SIZE], name[SIZE], surName[SIZE], department[SIZE]; // VARIABLES FOR READING FROM THE STUDENT FILE...

    while(!feof(pstudents))
    {
        // PART ONE .... THE HEADER...

        fscanf(pstudents,"%s", stdno);
        fscanf(pstudents,"%s", name);
        fscanf(pstudents,"%s", surName);
        fscanf(pstudents,"%s", department);

        //PRINTING IN THE TRANSCRIPT FILE

        fprintf(ptranscript, "Name :       %s\n", name);
        fprintf(ptranscript, "Surname :    %s\n", surName);
        fprintf(ptranscript, "Student No : %s\n", stdno);
        fprintf(ptranscript, "%s" ,"Department : ");

        // RETURN THE DEPARTMENT NAME...

        if (department [0] == 'E' && department [1] == 'C')
            fprintf(ptranscript,"%s", "Electronics and Communication Engineering\n");
        else if (department [0] == 'S' && department [1] == 'E')
            fprintf(ptranscript, "%s", "Software Engineering\n");
        else if (department[0] == 'C' && department[1] == 'O')
            fprintf(ptranscript, "%s", "Computer Enginnering\n");
        else if (department[0] == 'E' && department[1] == 'E')
            fprintf(ptranscript, "%s", "Electronics and Electrical Engineering\n");

        fprintf(ptranscript, "----------------------------------------------------------------------------------------------------------------");
        fprintf(ptranscript, "-----------------------------------------------------------------------------\n");

        // TYPEING THE ACADEMIC YEAR...

        char stdno01[SIZE], academic[SIZE], academic01[SIZE], courseCode[SIZE];  // VARIABLES FOR READING FROM THE STUDENT FILE...
        int semester = 0;

        fscanf(pregistrations, "%s", stdno01);
        fscanf(pregistrations, "%s", academic);// FOR THE '201920' ACADEMIC YEAR
        fscanf(pregistrations, "%d", &semester);
        fscanf(pregistrations, "%s", courseCode);

        fprintf(ptranscript, "Academic year: %0.4s-20\t", academic);//PRINTING JUST THE '2019'

        rewind(pregistrations);

        // SWITCH STATEMENT TO PRINT THE SEMESTER IN WORDS

        fprintf(ptranscript, "%s", "\tSemester : ");
        switch(semester)
        {
        case  1 :
            {
                fprintf(ptranscript, "%s\n", "Fall");
                break;
            }
        case  2 :
            {
                fprintf(ptranscript, "%s\n", "Spring");
                break;
            }
        default:
            fprintf(ptranscript, "%s\n", "NULL");
        }

        fprintf(ptranscript, "----------------------------------------------------------------------------------------------------------------");
        fprintf(ptranscript, "-----------------------------------------------------------------------------\n");

        // PART TWO ... THE DETAILS...

        //THE HEADER FOR THE PART TWO...
        fprintf(ptranscript, "%s", "Course Code");
        fprintf(ptranscript, "%18s", "Course Title");
        fprintf(ptranscript, "%33s", "Credit\t");
        fprintf(ptranscript, "%9s", "Grade");
        fprintf(ptranscript, "%17s", "Credit earned\n\n");

        char coursecode01[SIZE], courseTitle[SIZE];//  VARIABLES FOR READING FROM THE REGISTRATIONS FILE...
        rewind(pregistrations);

        do
        {
            fscanf(pregistrations, "%s", stdno01);
            fscanf(pregistrations, "%s", academic);
            fscanf(pregistrations, "%d", &semester);
            fscanf(pregistrations, "%s", courseCode);
            if(strcmp(stdno01, stdno) == 0)
            {
                fprintf(ptranscript,"%-16s", courseCode);// PRINTING THE COURSE CODE...
                do
                {
                    fscanf(pcourse, "%s", coursecode01);
                    fgets(courseTitle, SIZE, pcourse); // GETTING THE COURSE TITLE...
                    int len = strlen(courseTitle);
                    if ( courseTitle[len - 1] == '\n') // GETTING RID OF THE NEW IMPLICIT LINE CAUSING BY 'fgets'...
                        courseTitle[len - 1] = '\0';
                    if (strcmp(coursecode01, courseCode) == 0)
                    {
                        fprintf(ptranscript, "%-40s", courseTitle);
                        credit(ptranscript, pperformances,pcriteria, pstudents, pregistrations, coursecode01, stdno01);
                        fprintf(ptranscript, "\n");
                    }
                }while(!feof(pcourse));
            }
            rewind(pcourse);

        }while (!feof(pregistrations));

        fprintf(ptranscript, "----------------------------------------------------------------------------------------------------------------");
        fprintf(ptranscript, "-----------------------------------------------------------------------------\n");

        // PART THREE , THE CONCLUSION...

        GBA = credit_total / credit_hour;// GETTING THE GBA...

        fprintf(ptranscript, "Total credit earned: ");
        fprintf(ptranscript, "%0.1f\n", credit_total);

        credit_total = 0.0;// REINITIALIZE TO ZERO

        fprintf(ptranscript,"Total credit hour: ");
        fprintf(ptranscript, "%0.1f\n", credit_hour);

        credit_hour = 0.0;// REINITIALIZE TO ZERO

        fprintf(ptranscript,"GBA: ");
        fprintf(ptranscript, "%0.2f\n", GBA);

        GBA = 0.0;

        rewind(pregistrations);

        fprintf(ptranscript, "----------------------------------------------------------------------------------------------------------------");
        fprintf(ptranscript, "-----------------------------------------------------------------------------\n");

    }

    return 0;
}

// FOR TYPING THE CREDIT AND THE GRADE POILICY ( INCLUDE THE CREDIT EARNED AND THE GRADE LETTER )...
void credit(FILE * ptrancsript, FILE * pperformances, FILE * pcriteria, FILE * pstudents, FILE * pregistrations,
             char coursecode01[], char stdno01[])
{
    char courseCode002[SIZE];   // VARIABLES FOR READING FROM THE CRITERIA FILE...
    float lab, mid_term, final_term, credit_amount;

    while(!feof(pcriteria))
    {
        fscanf(pcriteria, "%s", courseCode002);
        fscanf(pcriteria, "%f", &credit_amount);
        fscanf(pcriteria, "%f", &lab);          // {
        fscanf(pcriteria, "%f", &mid_term);     //    THE PERFORMANCE MEASURES
        fscanf(pcriteria, "%f", &final_term);   // }

        if (strcmp(courseCode002, coursecode01) == 0)
        {
            fprintf(ptrancsript, "%-13.1f", credit_amount); // PRINTING THE CREDIT AMOUNT FOR EACH COURSE...
            grade( ptrancsript,pperformances,stdno01,courseCode002, lab, mid_term, final_term, credit_amount);
            break;
        }
    }

    rewind(pcriteria);

    credit_hour += credit_amount;// COLLECT THE CREDIT HOUR FOR EACH STUDENT....

    return;
}

// I AM GOING TO USE THIS FUNTION INSIDE THE 'CREDIT' FUNCTION...
void grade(FILE * ptrancsript, FILE * pperformances, char stdno01[],char courseCode002[],
             float lab, float mid_term, float final_term, float credit_amount)
{
    char stdno02[SIZE], courseCode003[SIZE];
    float lab_perf, mid_perf , final_perf, result;// THE ACTUAL PERFORMANCE FOR EACH STUDNET ON EACH SUBJECT
    float labTotal = 0, midTotal = 0, finalTotal = 0;// THE PERFORMANCE AFTER MULTIPLIED IT TO THE CRITERIA OF EACH COURSE
    float Numerical_grades = 0;


    while(!feof(pperformances))
    {

        fscanf(pperformances, "%s", stdno02);           // STUDNET NUMBER FROM PERFOMANCES
        fscanf(pperformances, "%s", courseCode003);     //  COURSE CODE FROM THE PERFOMANCES FILE
        fscanf(pperformances, "%f", &lab_perf);// THE LAB PERFORMANCES OF THE LABS WORK OR THE QUIZZES FOR ENGLISH
        fscanf(pperformances, "%f", &mid_perf);
        fscanf(pperformances, "%f", &final_perf);

        if (strcmp(stdno01, stdno02) == 0 && strcmp(courseCode002, courseCode003) == 0)
            break;
    }

    labTotal = lab_perf * ((float)lab/100);//LAB OR QUIZZES // {
    midTotal =  mid_perf * ((float)mid_term/100);           //   CALCULATIONS TO GET THE TOTAL SCORE FOR EACH COURSE...
    finalTotal = final_perf * ((float)final_term/100);      // }
    result = labTotal + midTotal +  finalTotal;

    // GRADING LETTER AND THE NUMERICAL SCORE FOR EACH GRADE LETTER...

    if(result > 90 && result < 100)
    {
        fprintf(ptrancsript, "%-10s" ,"A");
        Numerical_grades = 4.0;
    }

    else if (result >= 85 && result <= 89)
    {
        fprintf(ptrancsript,  "%-10s" ,"A-");
        Numerical_grades = 3.7;
    }

    else if (result >= 80 && result <=84)
    {
        fprintf(ptrancsript,  "%-10s" ,"B+");
        Numerical_grades = 3.3;
    }

    else if (result >= 75 && result <= 79)
    {
        fprintf(ptrancsript,  "%-10s" ,"B");
        Numerical_grades = 3.0;
    }

    else if (result >= 70 && result <= 74)
    {
        fprintf(ptrancsript,  "%-10s" ,"B-");
        Numerical_grades = 2.7;
    }

    else if (result >= 65 && result <= 69)
    {
        fprintf(ptrancsript,  "%-10s" ,"C+");
        Numerical_grades = 2.3;
    }

    else if (result >= 60 && result <= 64)
    {
        fprintf(ptrancsript,  "%-10s" ,"C");
        Numerical_grades = 2.0;
    }

    else if (result >= 55 && result <= 59)
    {
        fprintf(ptrancsript,  "%-10s" ,"C-");
        Numerical_grades = 1.7;
    }

    else if (result >= 50 && result <= 54)
    {
        fprintf(ptrancsript,  "%-10s" ,"D+");
        Numerical_grades = 1.3;
    }

    else if (result >= 45 && result <= 49)
    {
        fprintf(ptrancsript,  "%-10s" ,"D");
        Numerical_grades = 1.0;
    }

    else if (result >= 40 && result <= 44)
    {
        fprintf(ptrancsript,  "%-10s" ,"D-");
        Numerical_grades = 0.7;
    }

    else if (result >= 0 && result <= 39)
    {
        fprintf(ptrancsript,  "%-10s" ,"F");
        Numerical_grades = 0.0;
    }

    credit_earned = (float)Numerical_grades * (float)credit_amount; // GETTING THE CREDIT EARNED

    fprintf(ptrancsript, "%.1f\n", credit_earned);// PRINTING THE CREDIT EARNED

    credit_total += (float)credit_earned;// COLLECT THE CREDIT EARNED FOR EACH STUDENT THEN PRINT IT IN THE MAIN FUNCTION

    rewind(pperformances);

    return;
}
