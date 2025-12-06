#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <termios.h>
#include <unistd.h>
#define FILE_STU "records.txt"
typedef struct {
 int id; 
 char fullname[50]; 
 float score;
 } Record;
typedef struct {
 char uname[20]; 
 char pass[20]; 
 char role[15]; 
 } Account;
char activeUser[20];
char activeRole[20];
Account accountList[] = {
 {"admin", "admin123", "admin"},
 {"staff1", "staff123", "staff"},
 {"guest", "guest", "guest"}
};
int userCount = 3;
void preload_samples(); 
int verify_user(); 
void dashboard();
void menu_admin();
void menu_staff();
void menu_guest();
void insert_record();
void view_all();
void search_record(); 
void modify_record();
void remove_record();
void mask_password(char *pwd);
void mask_password(char *pwd) { 
 struct termios oldt, newt; 
 char ch;
 int idx = 0;
tcgetattr(STDIN_FILENO, &oldt);
newt = oldt;
newt.c_lflag &= ~(ECHO);
tcsetattr(STDIN_FILENO, TCSANOW, &newt);
while ((ch = getchar()) != '\n') {
 if (ch == 127 || ch == '\b') {
 if (idx > 0) {
 idx--;
 printf("\b \b");
 }
 } else {
 pwd[idx++] = ch;
 printf("*");
 }
}
pwd[idx] = '\0';
tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
printf("\n");
}
void preload_samples() {
 FILE *fp = fopen(FILE_STU, "r");
if (fp) { fclose(fp); return; }
fp = fopen(FILE_STU, "w");
if (!fp) { printf("Error creating file.\n"); return; }
fprintf(fp, "10 Rahul 88.5\n");
fprintf(fp, "11 Sneha 92.0\n");
fprintf(fp, "12 Karan 76.75\n");
fprintf(fp, "13 Priya 95.2\n");
fclose(fp);
printf("Sample student data loaded.\n");
}
int main() {
 preload_samples();
 if (verify_user())
 dashboard();
 else
 printf("Login failed! Exiting...\n");
 return 0;
}
int verify_user() {
 char uname[20], pwd[20];
printf("=========== USER LOGIN ===========\n");
printf("Username: ");
scanf("%s", uname);
printf("Password: ");
getchar();
mask_password(pwd);
for (int i = 0; i < userCount; i++) {
 if (strcmp(uname, accountList[i].uname) == 0 &&
 strcmp(pwd, accountList[i].pass) == 0) {
 strcpy(activeUser, uname);
 strcpy(activeRole, accountList[i].role);
 return 1;
 }
}
return 0;
}
void dashboard() {
 if (strcmp(activeRole, "admin") == 0)
 menu_admin();
 else if (strcmp(activeRole, "staff") == 0)
 menu_staff(); 
 else 
 menu_guest(); }
void menu_admin() {
 int option;
while (1) {
 printf("\n====== ADMIN PANEL ======\n");
 printf("1. Add Student\n");
 printf("2. View Students\n");
 printf("3. Search\n");
 printf("4. Edit Record\n");
 printf("5. Delete Record\n");
 printf("6. Logout\n");
 printf("Choose: ");
 scanf("%d", &option);
 switch (option) {
 case 1: insert_record(); break;
 case 2: view_all(); break;
 case 3: search_record(); break;
 case 4: modify_record(); break;
 case 5: remove_record(); break;
 case 6: return;
 default: printf("Invalid choice!\n");
 }
}
}
void menu_staff() {
int option;
while (1) {
 printf("\n====== STAFF MENU ======\n");
 printf("1. View Students\n");
 printf("2. Search\n");
 printf("3. Logout\n");
 printf("Choose: ");
 scanf("%d", &option);
 switch (option) {
 case 1: view_all(); break;
 case 2: search_record(); break;
 case 3: return;
 default: printf("Invalid choice!\n");
 }
 }
}
void menu_guest() { int option;
while (1) {
 printf("\n====== GUEST MENU ======\n");
 printf("1. View Students\n");
 printf("2. Logout\n");
 printf("Choose: ");
 scanf("%d", &option);
 switch (option) {
 case 1: view_all(); break;
 case 2: return;
 default: printf("Not allowed!\n");
 }
}
}
void insert_record() { 
Record r;
FILE *fp = fopen(FILE_STU, "a");
if (!fp) { 
printf("File error!\n"); 
return; 
}
printf("Enter ID: ");
scanf("%d", &r.id);
printf("Enter Name: ");
scanf("%s", r.fullname);
printf("Enter Score: ");
scanf("%f", &r.score);
fprintf(fp, "%d %s %.2f\n", r.id, r.fullname, r.score);
fclose(fp);
printf("Record Added.\n");
}
void view_all() {
Record r; 
FILE *fp = fopen(FILE_STU, "r");
if (!fp) { printf("No data available.\n"); return; }
printf("\n----- STUDENT RECORDS -----\n");
while (fscanf(fp, "%d %s %f", &r.id, r.fullname, &r.score) != EOF)
 printf("ID: %d | Name: %s | Marks: %.2f\n", r.id, r.fullname, 
r.score);
fclose(fp);
}
void search_record() {
int id, found = 0;
Record r;
printf("Enter ID to search: ");
scanf("%d", &id);
FILE *fp = fopen(FILE_STU, "r");
while (fscanf(fp, "%d %s %f", &r.id, r.fullname, &r.score) != EOF) {
 if (r.id == id) {
 printf("Record Found: %d | %s | %.2f\n", r.id, r.fullname, 
r.score);
 found = 1;
 break;
 }
}
fclose(fp);
if (!found)
 printf("No matching record found.\n");
}
void modify_record() {
int id, found = 0;
Record r;
printf("Enter ID to update: ");
scanf("%d", &id);
FILE *fp = fopen(FILE_STU, "r");
FILE *temp = fopen("tmp.txt", "w");
while (fscanf(fp, "%d %s %f", &r.id, r.fullname, &r.score) != EOF) {
 if (r.id == id) {
 printf("New Name: ");
 scanf("%s", r.fullname);
 printf("New Marks: ");
 scanf("%f", &r.score);
 found = 1;
 }
 fprintf(temp, "%d %s %.2f\n", r.id, r.fullname, r.score);
}
fclose(fp);
fclose(temp);
remove(FILE_STU);
rename("tmp.txt", FILE_STU);
if (found) printf("Updated Successfully!\n");
else printf("Record not found.\n");
}
void remove_record() { 
int id, found = 0; 
Record r;
printf("Enter ID to delete: ");
scanf("%d", &id);
FILE *fp = fopen(FILE_STU, "r");
FILE *temp = fopen("tmp.txt", "w");
while (fscanf(fp, "%d %s %f", &r.id, r.fullname, &r.score) != EOF) {
 if (r.id == id) {
 found = 1;
 continue;
 }
 fprintf(temp, "%d %s %.2f\n", r.id, r.fullname, r.score);
}
fclose(fp);
fclose(temp);
remove(FILE_STU);
rename("tmp.txt", FILE_STU);
if (found) printf("Record Deleted!\n");
else printf("Record not found.\n");
}