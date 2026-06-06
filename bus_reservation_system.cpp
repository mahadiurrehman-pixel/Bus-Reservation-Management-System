#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <stdexcept>
using namespace std;
// we define some name soo we can use in code laterly
#define MAX_SEATS 100
#define MAX_BUSES 100
#define MAX_ATTEMPTS 3
#define ACCOUNTS_FILE "accounts.txt"
#define BUSES_FILE "buses.txt"
#define SEATS_FILE "seats.txt"
#define LOG_FILE "activity_log.txt"
#define ADMIN_FILE "admin_log.txt"
#define REVIEWS_FILE "reviews.txt"
#define DESTROYED_FILE "destroyed_buses.txt"
#define REFUNDS_FILE "refunds.txt"
#define FINANCE_FILE "finance.txt"
// GLobal Variable
string loguser;
string logpass;
//Reg are use temporaily
string regusername;
string userpassword;
// STRUCT WE ARE USING
struct Bus {
string busNo;
string type;
int seats;
string route;
string price;
string departure;
string arrival;
};

struct User {
string username;
string password;
};

struct Booking {
string username;
string busNo;
string seatNo;
string token;
};

string getValidPassword();// it will check the password length uppercase and special character etc
void admin_login(); // Mainly admin login it will ask admin user and adminpass
void admin_menu(); // admin menu will display and ask admin what he want to do
void add_bus(); // this function is to add bus
void sortBuses(Bus arr[], int n, int sortBy); // it will help in sorting bus use in book bus , bus detail , route time
void view_logs(); // this function is use to see login account
void logActivity(const string& activity); // this function will display log activity
void remove_bus(); // this function is use to remove bus and here we use temp file logic
bool MazbootPassword(const string& pass); // Mazboot password will check the password length uppercase and special character etc
void remove_account(); // this function is to remove accs like in this we use temp txt file logic too
void change_password(); // this is use to change password and it also contain temp file logic
void view_users(); // it will be use to registered acc
void view_bookings(); // it will display the bookings
string generateToken(const string& busNo, const string& seatNo, const string& username);// this will create token
void register_(); // it is a user function and use to register new account
void realmenu(); // this will display user menu
void book_seat(); // this will use to book seat and user will seek bus detail and display token generated
void log_in(); // it's a user function and it will allow user to log in
void Menu(); // this is a login interface which ask user what u wanna do and it will ask user if u want to login register or login as admin
void route_time(); // this will display route timing and here we also use sorting function
void check_booking(); // this will allow user to check booking
void bus_detail(); // it will display bus detail and here we also use sorting function
void ent_rev(); // this function will allow user to enter the reviews
void oth_rev(); // this function will allow user to see other ppls review
void review_(); // review function is mainly the review menu which will ask user what he want to do like user want to enter review or see other review
void cancel_booking(); // this function will allow user to cancel the booking
void user_change_password(); // this will allow user to change the user password
void search_route(); // this will allow user to search route by name , number and etc
void my_bookings(); // this will allow user to check their bookings
void edit_bus(); // this will allow user to edit the bus and etc
void destroy_bus(); // this will destroy bus and all the refunds will be deliver to users
bool isDestroyedBus(string busNo); // this will check is the bus destroyed
void view_refunds(); // this will allow admin to view refund
void approve_refund(); // by this the admin can approve refund
void cancel_refund(); // by this the admin can cancel
void my_refunds(); // by this the user can view refunds
void finance_login(); // this will allow finance user to login
void finance_menu(); // this will display finance menu
void monthly_revenue(); // this will show monthly revenue report
void refund_loss(); // this will show refund loss report
void net_profit(); // this will show net profit report
void finance_view_refunds(); // this will allow finance to view refunds
void finance_approve_refund(); // this will allow finance to approve refund
void finance_cancel_refund(); // this will allow finance to cancel refund

void ClearScreen() // this function is to clear previous screen
{
#ifdef _WIN32 // this is preprocessor directive and it will check that it'll runing in window or not
system("cls");// if it is wwindow then the terminal will be clean
#else
system("clear"); // it will do the same thing but in linux or mac
#endif
}
void PauseScreen()
{
cout << "\nPress Enter To Continue...";
cin.ignore(); // this will clear buffer ignore() use leftover Enter ko remove karta hai
cin.get(); //it will pause the program and wait till the user press the enter
}

bool isAllDigits(const string& str) //Function use : check if string has pure or not .
// isALLDiggits is from #include <cctype> we didnt include it as the program was runing smoothly
{
if(str.empty()) return false;
for(int i = 0; i < str.length(); i++) // it will check every character
{
if(!isdigit(str[i])) return false;
}
return true;
}
// remaing are like isAlldigits
bool isValidName(const string& str){
if(str.empty()) return false;
for(int i = 0; i < str.length(); i++){
if(!isalnum(str[i]) && str[i] != '_') return false;
}
return true;
}

bool isValidBusNo(const string& str){
if(str.empty()) return false;
for(int i = 0; i < str.length(); i++){
if(!isalnum(str[i]) && str[i] != '-') return false;
}
return true;
}

bool isValidRoute(const string& str){
if(str.empty()) return false;
for(int i = 0; i < str.length(); i++){
if(!isalpha(str[i]) && str[i] != '-') return false;
}
return true;
}

bool isValidTime(const string& str){
if(str.length() < 5) return false;
return true;
}

int stringToInt(const string& str){
if(str.empty()) throw runtime_error("Cannot Convert Empty String To Integer");
for(int i = 0; i < str.length(); i++){
if(!isdigit(str[i])) throw runtime_error("Invalid Number Format: " + str);
}
int val = 0;
for(int i = 0; i < str.length(); i++)
val = val * 10 + (str[i] - '0');
// logic : if val is 3 then it ascii value is 51 and zero ascii value is 48 and the substraction of 51-48=3 which is conversion
return val;
}
// limitaton
// cant handle negative numbers
string intToString(int val){
if(val == 0) return "0";
string result = "";
while(val > 0){
result = (char)('0' + val % 10) + result;
val /= 10;
}
return result;
}

int getValidInt(int minVal, int maxVal){
string input;
int value;
while(true){
cin >> input;
if(!isAllDigits(input)){
cout << "Invalid Input! Enter A Number Between " << minVal << " And " << maxVal << ": ";
continue;
}
try{
value = stringToInt(input);
}
catch(runtime_error&){
cout << "Invalid Input! Enter A Number Between " << minVal << " And " << maxVal << ": ";
continue;
}
if(value < minVal || value > maxVal){
cout << "Out Of Range! Enter Between " << minVal << " And " << maxVal << ": ";
continue;
}
return value;
}
}

int getValidSeatNo(int maxSeats, int bookedSeats[], int bookedCount){
string input;
while(true){
cin >> input;

if(!isAllDigits(input)){
cout << "Invalid! Enter Numbers Only (1-" << maxSeats << "): ";
continue;
}

if(input.length() > 3){
cout << "Invalid Seat Number! Too Long. Enter (1-" << maxSeats << "): ";
continue;
}

int seatNum;
try{
seatNum = stringToInt(input);
}
catch(runtime_error&){
cout << "Invalid Seat Number! Enter (1-" << maxSeats << "): ";
continue;
}

if(seatNum < 1){
cout << "Seat Cannot Be 0 Or Negative! Enter (1-" << maxSeats << "): ";
continue;
}

if(seatNum > maxSeats){
cout << "Seat " << seatNum << " Does Not Exist! This Bus Has Only " << maxSeats << " Seats. Enter (1-" << maxSeats << "): ";
continue;
}

bool alreadyBooked = false;
for(int j = 0; j < bookedCount; j++){
if(bookedSeats[j] == seatNum){
alreadyBooked = true;
break;
}
}
if(alreadyBooked){
cout << "Seat " << seatNum << " Is Already Booked! Choose Another Seat (1-" << maxSeats << "): ";
continue;
}

return seatNum;
}
}

string getValidToken(){
string input;
while(true){
cin >> input;
if(input.empty()){
cout << "Token Cannot Be Empty! Try Again: ";
continue;
}
bool valid = true;
for(int i = 0; i < input.length(); i++){
if(isspace(input[i])){
valid = false;
break;
}
}
if(!valid){
cout << "Invalid Token Format! Try Again: ";
continue;
}
return input;
}
}

int main(){
try{
Menu();
}
catch(runtime_error& e){
cout << "\n[Runtime Error] " << e.what() << endl;
PauseScreen();
}
catch(exception& e){
cout << "\n[Error] " << e.what() << endl;
PauseScreen();
}
catch(...){
cout << "\n[Unknown Error] Something Went Wrong!\n";
PauseScreen();
}
return 0;
}

bool isDestroyedBus(string busNo){
ifstream file(DESTROYED_FILE);
if(!file.is_open()) return false;
string b, t, s, r, p, dep, arr, status;
while(file >> b >> t >> s >> r >> p >> dep >> arr >> status){
if(b == busNo && status == "DESTROYED"){
file.close();
return true;
}
}
file.close();
return false;
}

void add_bus(){
Bus b;
string seatsInput;
ClearScreen();
cout << "\n===Add Bus===\n";

while(true){
cout << "Enter Bus Number: ";
cin >> b.busNo;
if(!isValidBusNo(b.busNo)){
cout << "Invalid Bus Number! Use Letters, Digits Or Hyphen Only.\n";
continue;
}
if(isDestroyedBus(b.busNo)){
cout << "This bus was permanently destroyed and cannot be reused\n";
continue;
}
break;
}

while(true){
cout << "Enter Type (AC/NonAC): ";
cin >> b.type;
if(b.type != "AC" && b.type != "NonAC"){
cout << "Invalid Type! Enter AC Or NonAC.\n";
continue;
}
break;
}

while(true){
cout << "Enter Total Seats (1-100): ";
cin >> seatsInput;
if(!isAllDigits(seatsInput)){
cout << "Invalid! Seats Must Be A Number.\n";
continue;
}
try{
b.seats = stringToInt(seatsInput);
}
catch(runtime_error& e){
cout << "Conversion Error: " << e.what() << "\n";
continue;
}
if(b.seats < 1 || b.seats > 100){
cout << "Seats Must Be Between 1 And 100.\n";
continue;
}
break;
}

while(true){
cout << "Enter Route (e.g Karachi-Lahore): ";
cin >> b.route;
if(!isValidRoute(b.route)){
cout << "Invalid Route! Use Letters And Hyphen Only.\n";
continue;
}
break;
}

while(true){
cout << "Enter Ticket Price (Rs): ";
cin >> b.price;
if(!isAllDigits(b.price) || b.price.empty()){
cout << "Invalid Price! Enter Numbers Only.\n";
continue;
}
break;
}

while(true){
cout << "Enter Departure Time (e.g 08:00AM): ";
cin >> b.departure;
if(!isValidTime(b.departure)){
cout << "Invalid Time Format!\n";
continue;
}
break;
}

while(true){
cout << "Enter Arrival Time (e.g 06:00PM): ";
cin >> b.arrival;
if(!isValidTime(b.arrival)){
cout << "Invalid Time Format!\n";
continue;
}
break;
}

try{
ofstream file(BUSES_FILE, ios::app);
if(!file.is_open()) throw runtime_error("Failed To Open Buses File For Writing");
file << b.busNo << " " << b.type << " "
<< b.seats << " " << b.route << " "
<< b.price << " " << b.departure << " "
<< b.arrival << endl;
file.close();
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
PauseScreen();
return;
}
logActivity("Bus Added: " + b.busNo);
cout << "Bus Added Successfully!\n";
PauseScreen();
}

void sortBuses(Bus arr[], int n, int sortBy){
for(int i = 0; i < n - 1; i++){
for(int j = 0; j < n - i - 1; j++){
bool swapNeeded = false;
if(sortBy == 1){
int p1, p2;
try{
p1 = stringToInt(arr[j].price);
p2 = stringToInt(arr[j+1].price);
}
catch(runtime_error&){
continue;
}
if(p1 > p2) swapNeeded = true;
}
else if(sortBy == 2){
if(arr[j].route > arr[j+1].route) swapNeeded = true;
}
else if(sortBy == 3){
if(arr[j].busNo > arr[j+1].busNo) swapNeeded = true;
}
if(swapNeeded){
Bus temp = arr[j];
arr[j] = arr[j+1];
arr[j+1] = temp;
}
}
}
}

void remove_bus(){
string busNo, b, t, s, r, p, d, a;
ClearScreen();
cout << "\n===Remove Bus===\n";

while(true){
cout << "Enter Bus Number To Remove: ";
cin >> busNo;
if(!isValidBusNo(busNo)){
cout << "Invalid Bus Number Format!\n";
continue;
}
break;
}

try{
ifstream fin(BUSES_FILE);
if(!fin.is_open()) throw runtime_error("Failed To Open Buses File For Reading");
ofstream fout("temp.txt");
if(!fout.is_open()){
fin.close();
throw runtime_error("Failed To Open Temp File For Writing");
}
bool found = false;

while(fin >> b >> t >> s >> r >> p >> d >> a){
if(b != busNo){
fout << b << " " << t << " "
<< s << " " << r << " " << p << " " << d << " " << a << endl;
}
else{
found = true;
}
}
fin.close();
fout.close();

if(remove(BUSES_FILE) != 0) throw runtime_error("Failed To Remove Old Buses File");
if(rename("temp.txt", BUSES_FILE) != 0) throw runtime_error("Failed To Rename Temp File");

if(found) cout << "Bus Removed!\n";
else cout << "Bus Not Found!\n";
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
}
PauseScreen();
}

void remove_account(){
string target, u, p;
ClearScreen();
cout << "\n===Remove Account===\n";

while(true){
cout << "Enter Username To Remove: ";
cin >> target;
if(!isValidName(target)){
cout << "Invalid Username Format!\n";
continue;
}
break;
}

try{
ifstream fin(ACCOUNTS_FILE);
if(!fin.is_open()) throw runtime_error("Failed To Open Accounts File For Reading");
ofstream fout("temp.txt");
if(!fout.is_open()){
fin.close();
throw runtime_error("Failed To Open Temp File For Writing");
}
bool found = false;

while(fin >> u >> p){
if(u != target){
fout << u << " " << p << endl;
}
else{
found = true;
}
}
fin.close();
fout.close();

if(remove(ACCOUNTS_FILE) != 0) throw runtime_error("Failed To Remove Old Accounts File");
if(rename("temp.txt", ACCOUNTS_FILE) != 0) throw runtime_error("Failed To Rename Temp File");

if(found) cout << "Account Removed!\n";
else cout << "User Not Found!\n";
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
}
PauseScreen();
}

void change_password(){
string target, newPass, u, p;
ClearScreen();
cout << "\n===Change Password===\n";

while(true){
cout << "Enter Username: ";
cin >> target;
if(!isValidName(target)){
cout << "Invalid Username Format!\n";
continue;
}
break;
}

while(true){
cout << "Enter New Password: ";
cin >> newPass;
if(!MazbootPassword(newPass)){
cout << "\n===Weak Password!===\n";
cout << "Password should contain :\n";
cout << "- Atleast 8 characters\n";
cout << "- One Capital Letter (A-Z)\n";
cout << "- One Number (0-9)\n";
cout << "- One Special Character (!@#$)\n";
cout << "Example: Ali@1234\n";
continue;
}
break;
}

try{
ifstream fin(ACCOUNTS_FILE);
if(!fin.is_open()) throw runtime_error("Failed To Open Accounts File For Reading");
ofstream fout("temp.txt");
if(!fout.is_open()){
fin.close();
throw runtime_error("Failed To Open Temp File For Writing");
}
bool found = false;
while(fin >> u >> p){
if(u == target){
fout << u << " " << newPass << endl;
found = true;
}
else{
fout << u << " " << p << endl;
}
}
fin.close();
fout.close();

if(remove(ACCOUNTS_FILE) != 0) throw runtime_error("Failed To Remove Old Accounts File");
if(rename("temp.txt", ACCOUNTS_FILE) != 0) throw runtime_error("Failed To Rename Temp File");

if(found) cout << "Password Changed!\n";
else cout << "User Not Found!\n";
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
}
PauseScreen();
}

void logActivity(const string& activity){
try{
ofstream log(LOG_FILE, ios::app);
if(!log.is_open()) throw runtime_error("Failed To Open Log File");
time_t now = time(0);
string dt = ctime(&now);
dt = dt.substr(0, dt.length()-1);
log << "[" << dt << "] "
<< "[" << loguser << "] "
<< activity << endl;
log.close();
}
catch(runtime_error& e){
cout << "[Log Error] " << e.what() << "\n";
}
}

void view_logs(){
ClearScreen();
try{
ifstream log(LOG_FILE);
if(!log.is_open()) throw runtime_error("Failed To Open Log File For Reading");
string line;
int count = 1;
bool empty = true;
cout << "\n===Activity Log===\n";
cout << "----------------------------------------\n";
while(getline(log, line)){
cout << count << ". " << line << endl;
count++;
empty = false;
}
if(empty) cout << "No Activity Yet!\n";
log.close();
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
}
PauseScreen();
}

void view_users(){
ClearScreen();
cout << "\n===All Users===\n";
try{
ifstream file(ACCOUNTS_FILE);
if(!file.is_open()) throw runtime_error("Failed To Open Accounts File For Reading");
string u, p;
int count = 1;
bool empty = true;

while(file >> u >> p){
cout << count << ". " << u << endl;
count++;
empty = false;
}
file.close();
if(empty) cout << "No Users Found!\n";
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
}
PauseScreen();
}

void view_bookings(){
ClearScreen();
cout << "\n===All Bookings===\n";
try{
ifstream file(SEATS_FILE);
if(!file.is_open()) throw runtime_error("Failed To Open Seats File For Reading");
string line;
int count = 1;
bool empty = true;
while(getline(file, line)){
cout << count << ". " << line << endl;
count++;
empty = false;
}
file.close();
if(empty) cout << "No Bookings Yet!\n";
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
}
PauseScreen();
}

void destroy_bus(){
ClearScreen();
cout << "\n===Destroy Bus===\n";
string busNo;

while(true){
cout << "Enter Bus Number To Destroy: ";
cin >> busNo;
if(!isValidBusNo(busNo)){
cout << "Invalid Bus Number Format!\n";
continue;
}
break;
}

string b, t, s, r, p, dep, arr;
bool busFound = false;
string savedType, savedSeats, savedRoute, savedPrice, savedDep, savedArr;

try{
ifstream busRead(BUSES_FILE);
if(!busRead.is_open()) throw runtime_error("Failed To Open Buses File For Reading");
while(busRead >> b >> t >> s >> r >> p >> dep >> arr){
if(b == busNo){
busFound = true;
savedType = t;
savedSeats = s;
savedRoute = r;
savedPrice = p;
savedDep = dep;
savedArr = arr;
break;
}
}
busRead.close();
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
PauseScreen();
return;
}

if(!busFound){
cout << "Bus Not Found!\n";
PauseScreen();
return;
}

try{
ifstream finBus(BUSES_FILE);
if(!finBus.is_open()) throw runtime_error("Failed To Open Buses File For Reading");
ofstream foutBus("temp.txt");
if(!foutBus.is_open()){
finBus.close();
throw runtime_error("Failed To Open Temp File For Writing");
}
while(finBus >> b >> t >> s >> r >> p >> dep >> arr){
if(b != busNo){
foutBus << b << " " << t << " " << s << " " << r << " " << p << " " << dep << " " << arr << endl;
}
}
finBus.close();
foutBus.close();

if(remove(BUSES_FILE) != 0) throw runtime_error("Failed To Remove Old Buses File");
if(rename("temp.txt", BUSES_FILE) != 0) throw runtime_error("Failed To Rename Temp File");
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
PauseScreen();
return;
}

try{
ofstream destroyed(DESTROYED_FILE, ios::app);
if(!destroyed.is_open()) throw runtime_error("Failed To Open Destroyed Buses File");
destroyed << busNo << " "
<< savedType << " "
<< savedSeats << " "
<< savedRoute << " "
<< savedPrice << " "
<< savedDep << " "
<< savedArr << " "
<< "DESTROYED" << endl;
destroyed.close();
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
PauseScreen();
return;
}

logActivity("Bus Destroyed: " + busNo);

string su, sb, ss, st;
try{
ifstream seatRead(SEATS_FILE);
if(!seatRead.is_open()) throw runtime_error("Failed To Open Seats File For Reading");

while(seatRead >> su >> sb >> ss >> st){
if(sb == busNo){
ofstream refFile(REFUNDS_FILE, ios::app);
if(!refFile.is_open()) throw runtime_error("Failed To Open Refunds File For Writing");
refFile << su << " "
<< sb << " "
<< ss << " "
<< st << " "
<< savedPrice << " "
<< "Pending" << endl;
refFile.close();
logActivity("Refund Created - User:" + su + " Bus:" + sb + " Seat:" + ss);
}
}
seatRead.close();
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
PauseScreen();
return;
}

try{
ifstream finSeat(SEATS_FILE);
if(!finSeat.is_open()) throw runtime_error("Failed To Open Seats File For Reading");
ofstream foutSeat("temp.txt");
if(!foutSeat.is_open()){
finSeat.close();
throw runtime_error("Failed To Open Temp File For Writing");
}
while(finSeat >> su >> sb >> ss >> st){
if(sb != busNo){
foutSeat << su << " " << sb << " " << ss << " " << st << endl;
}
}
finSeat.close();
foutSeat.close();

if(remove(SEATS_FILE) != 0) throw runtime_error("Failed To Remove Old Seats File");
if(rename("temp.txt", SEATS_FILE) != 0) throw runtime_error("Failed To Rename Temp File");
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
PauseScreen();
return;
}

cout << "Bus Destroyed And Refunds Created For All Passengers!\n";
PauseScreen();
}

void view_refunds(){
ClearScreen();
cout << "\n===All Refunds===\n";
try{
ifstream file(REFUNDS_FILE);
if(!file.is_open()) throw runtime_error("Failed To Open Refunds File For Reading");
string u, b, s, t, amt, status;
int count = 1;
bool empty = true;
while(file >> u >> b >> s >> t >> amt >> status){
cout << "--------------------------------\n";
cout << count << ". User : " << u << endl;
cout << " Bus : " << b << endl;
cout << " Seat : " << s << endl;
cout << " Token : " << t << endl;
cout << " Amount : Rs." << amt << endl;
cout << " Status : " << status << endl;
count++;
empty = false;
}
file.close();
if(empty) cout << "No Refunds Found!\n";
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
}
PauseScreen();
}

void approve_refund(){
ClearScreen();
view_refunds();
cout << "\n===Approve Refund===\n";
cout << "Enter Token To Approve: ";
string token = getValidToken();

try{
ifstream fin(REFUNDS_FILE);
if(!fin.is_open()) throw runtime_error("Failed To Open Refunds File For Reading");
ofstream fout("temp.txt");
if(!fout.is_open()){
fin.close();
throw runtime_error("Failed To Open Temp File For Writing");
}
string u, b, s, t, amt, status;
bool found = false;

while(fin >> u >> b >> s >> t >> amt >> status){
if(t == token && status == "Pending"){
fout << u << " " << b << " " << s << " " << t << " " << amt << " " << "Approved" << endl;
found = true;
cout << "Refund Approved For User: " << u << "\n";
logActivity("Refund Approved - Token:" + token);
}
else{
fout << u << " " << b << " " << s << " " << t << " " << amt << " " << status << endl;
}
}
fin.close();
fout.close();

if(remove(REFUNDS_FILE) != 0) throw runtime_error("Failed To Remove Old Refunds File");
if(rename("temp.txt", REFUNDS_FILE) != 0) throw runtime_error("Failed To Rename Temp File");

if(!found) cout << "Token Not Found Or Already Processed!\n";
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
}
PauseScreen();
}

void cancel_refund(){
ClearScreen();
view_refunds();
cout << "\n===Cancel Refund===\n";
cout << "Enter Token To Cancel: ";
string token = getValidToken();

try{
ifstream fin(REFUNDS_FILE);
if(!fin.is_open()) throw runtime_error("Failed To Open Refunds File For Reading");
ofstream fout("temp.txt");
if(!fout.is_open()){
fin.close();
throw runtime_error("Failed To Open Temp File For Writing");
}
string u, b, s, t, amt, status;
bool found = false;

while(fin >> u >> b >> s >> t >> amt >> status){
if(t == token && status == "Pending"){
fout << u << " " << b << " " << s << " " << t << " " << amt << " " << "Cancelled" << endl;
found = true;
cout << "Refund Cancelled For User: " << u << "\n";
logActivity("Refund Cancelled - Token:" + token);
}
else{
fout << u << " " << b << " " << s << " " << t << " " << amt << " " << status << endl;
}
}
fin.close();
fout.close();

if(remove(REFUNDS_FILE) != 0) throw runtime_error("Failed To Remove Old Refunds File");
if(rename("temp.txt", REFUNDS_FILE) != 0) throw runtime_error("Failed To Rename Temp File");

if(!found) cout << "Token Not Found Or Already Processed!\n";
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
}
PauseScreen();
}

void my_refunds(){
ClearScreen();
cout << "\n===My Refunds===\n";
try{
ifstream file(REFUNDS_FILE);
if(!file.is_open()) throw runtime_error("Failed To Open Refunds File For Reading");
string u, b, s, t, amt, status;
int count = 1;
bool found = false;
while(file >> u >> b >> s >> t >> amt >> status){
if(u == loguser){
cout << "--------------------------------\n";
cout << count << ". Token : " << t << endl;
cout << " Bus : " << b << endl;
cout << " Seat : " << s << endl;
cout << " Amount : Rs." << amt << endl;
cout << " Status : " << status << endl;
count++;
found = true;
}
}
file.close();
if(!found) cout << "No Refunds Found!\n";
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
}
PauseScreen();
}

void monthly_revenue(){
ClearScreen();
cout << "\n===Monthly Revenue Report===\n";

string busNos[MAX_BUSES];
int busPrices[MAX_BUSES];
int busBookings[MAX_BUSES];
int busRevenues[MAX_BUSES];
int busCount = 0;

try{
ifstream busFile(BUSES_FILE);
if(!busFile.is_open()) throw runtime_error("Failed To Open Buses File For Reading");
string b, t, s, r, p, dep, arr;
while(busFile >> b >> t >> s >> r >> p >> dep >> arr){
busNos[busCount] = b;
try{
busPrices[busCount] = stringToInt(p);
}
catch(runtime_error&){
busPrices[busCount] = 0;
}
busBookings[busCount] = 0;
busRevenues[busCount] = 0;
busCount++;
}
busFile.close();
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
PauseScreen();
return;
}

int totalBookings = 0;
int totalRevenue = 0;

try{
ifstream seatFile(SEATS_FILE);
if(!seatFile.is_open()) throw runtime_error("Failed To Open Seats File For Reading");
string su, sb, ss, st;
while(seatFile >> su >> sb >> ss >> st){
for(int i = 0; i < busCount; i++){
if(busNos[i] == sb){
busBookings[i]++;
busRevenues[i] += busPrices[i];
totalBookings++;
totalRevenue += busPrices[i];
break;
}
}
}
seatFile.close();
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
PauseScreen();
return;
}

string mostBookedBus = "None";
int maxBookings = 0;
for(int i = 0; i < busCount; i++){
if(busBookings[i] > maxBookings){
maxBookings = busBookings[i];
mostBookedBus = busNos[i];
}
}

cout << "----------------------------------------\n";
cout << "Bus-Wise Revenue:\n";
cout << "----------------------------------------\n";
for(int i = 0; i < busCount; i++){
cout << "Bus : " << busNos[i]
<< " | Bookings : " << busBookings[i]
<< " | Revenue : Rs." << busRevenues[i] << endl;
}
cout << "----------------------------------------\n";
cout << "Total Bookings  : " << totalBookings << endl;
cout << "Total Revenue   : Rs." << totalRevenue << endl;
cout << "Most Booked Bus : " << mostBookedBus << endl;
cout << "----------------------------------------\n";
PauseScreen();
}

void refund_loss(){
ClearScreen();
cout << "\n===Refund Loss Report===\n";

string refBusNos[MAX_BUSES];
int refBusCount[MAX_BUSES];
int refBusTotal = 0;
int totalApproved = 0;
int totalLoss = 0;

try{
ifstream file(REFUNDS_FILE);
if(!file.is_open()) throw runtime_error("Failed To Open Refunds File For Reading");
string u, b, s, t, amt, status;
while(file >> u >> b >> s >> t >> amt >> status){
if(status == "Approved"){
totalApproved++;
try{
totalLoss += stringToInt(amt);
}
catch(runtime_error&){
totalLoss += 0;
}
bool found = false;
for(int i = 0; i < refBusTotal; i++){
if(refBusNos[i] == b){
refBusCount[i]++;
found = true;
break;
}
}
if(!found){
refBusNos[refBusTotal] = b;
refBusCount[refBusTotal] = 1;
refBusTotal++;
}
}
}
file.close();
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
PauseScreen();
return;
}

string mostRefundedBus = "None";
int maxRef = 0;
for(int i = 0; i < refBusTotal; i++){
if(refBusCount[i] > maxRef){
maxRef = refBusCount[i];
mostRefundedBus = refBusNos[i];
}
}

cout << "----------------------------------------\n";
cout << "Total Approved Refunds : " << totalApproved << endl;
cout << "Total Refund Loss      : Rs." << totalLoss << endl;
cout << "Most Refunded Bus      : " << mostRefundedBus << endl;
cout << "----------------------------------------\n";
PauseScreen();
}

void net_profit(){
ClearScreen();
cout << "\n===Net Profit Report===\n";

string busNos[MAX_BUSES];
int busPrices[MAX_BUSES];
int busCount = 0;

try{
ifstream busFile(BUSES_FILE);
if(!busFile.is_open()) throw runtime_error("Failed To Open Buses File For Reading");
string b, t, s, r, p, dep, arr;
while(busFile >> b >> t >> s >> r >> p >> dep >> arr){
busNos[busCount] = b;
try{
busPrices[busCount] = stringToInt(p);
}
catch(runtime_error&){
busPrices[busCount] = 0;
}
busCount++;
}
busFile.close();
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
PauseScreen();
return;
}

int totalRevenue = 0;

try{
ifstream seatFile(SEATS_FILE);
if(!seatFile.is_open()) throw runtime_error("Failed To Open Seats File For Reading");
string su, sb, ss, st;
while(seatFile >> su >> sb >> ss >> st){
for(int i = 0; i < busCount; i++){
if(busNos[i] == sb){
totalRevenue += busPrices[i];
break;
}
}
}
seatFile.close();
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
PauseScreen();
return;
}

int totalLoss = 0;

try{
ifstream refFile(REFUNDS_FILE);
if(!refFile.is_open()) throw runtime_error("Failed To Open Refunds File For Reading");
string ru, rb, rs, rt, ramt, rstatus;
while(refFile >> ru >> rb >> rs >> rt >> ramt >> rstatus){
if(rstatus == "Approved"){
try{
totalLoss += stringToInt(ramt);
}
catch(runtime_error&){
totalLoss += 0;
}
}
}
refFile.close();
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
PauseScreen();
return;
}

int finalProfit = totalRevenue - totalLoss;

cout << "----------------------------------------\n";
cout << "Total Revenue  : Rs." << totalRevenue << endl;
cout << "Refund Loss    : Rs." << totalLoss << endl;
cout << "----------------------------------------\n";
cout << "Net Profit     : Rs." << finalProfit << endl;
cout << "----------------------------------------\n";
PauseScreen();
}

void finance_view_refunds(){
ClearScreen();
cout << "\n===All Refunds===\n";
try{
ifstream file(REFUNDS_FILE);
if(!file.is_open()) throw runtime_error("Failed To Open Refunds File For Reading");
string u, b, s, t, amt, status;
int count = 1;
bool empty = true;
while(file >> u >> b >> s >> t >> amt >> status){
cout << "--------------------------------\n";
cout << count << ". User : " << u << endl;
cout << " Bus : " << b << endl;
cout << " Seat : " << s << endl;
cout << " Token : " << t << endl;
cout << " Amount : Rs." << amt << endl;
cout << " Status : " << status << endl;
count++;
empty = false;
}
file.close();
if(empty) cout << "No Refunds Found!\n";
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
}
PauseScreen();
}

void finance_approve_refund(){
ClearScreen();
finance_view_refunds();
cout << "\n===Approve Refund===\n";
cout << "Enter Token To Approve: ";
string token = getValidToken();

try{
ifstream fin(REFUNDS_FILE);
if(!fin.is_open()) throw runtime_error("Failed To Open Refunds File For Reading");
ofstream fout("temp.txt");
if(!fout.is_open()){
fin.close();
throw runtime_error("Failed To Open Temp File For Writing");
}
string u, b, s, t, amt, status;
bool found = false;

while(fin >> u >> b >> s >> t >> amt >> status){
if(t == token && status == "Pending"){
fout << u << " " << b << " " << s << " " << t << " " << amt << " " << "Approved" << endl;
found = true;
cout << "Refund Approved For User: " << u << "\n";
logActivity("Refund Approved - Token:" + token);
}
else{
fout << u << " " << b << " " << s << " " << t << " " << amt << " " << status << endl;
}
}
fin.close();
fout.close();

if(remove(REFUNDS_FILE) != 0) throw runtime_error("Failed To Remove Old Refunds File");
if(rename("temp.txt", REFUNDS_FILE) != 0) throw runtime_error("Failed To Rename Temp File");

if(!found) cout << "Token Not Found Or Already Processed!\n";
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
}
PauseScreen();
}

void finance_cancel_refund(){
ClearScreen();
finance_view_refunds();
cout << "\n===Cancel Refund===\n";
cout << "Enter Token To Cancel: ";
string token = getValidToken();

try{
ifstream fin(REFUNDS_FILE);
if(!fin.is_open()) throw runtime_error("Failed To Open Refunds File For Reading");
ofstream fout("temp.txt");
if(!fout.is_open()){
fin.close();
throw runtime_error("Failed To Open Temp File For Writing");
}
string u, b, s, t, amt, status;
bool found = false;

while(fin >> u >> b >> s >> t >> amt >> status){
if(t == token && status == "Pending"){
fout << u << " " << b << " " << s << " " << t << " " << amt << " " << "Cancelled" << endl;
found = true;
cout << "Refund Cancelled For User: " << u << "\n";
logActivity("Refund Cancelled - Token:" + token);
}
else{
fout << u << " " << b << " " << s << " " << t << " " << amt << " " << status << endl;
}
}
fin.close();
fout.close();

if(remove(REFUNDS_FILE) != 0) throw runtime_error("Failed To Remove Old Refunds File");
if(rename("temp.txt", REFUNDS_FILE) != 0) throw runtime_error("Failed To Rename Temp File");

if(!found) cout << "Token Not Found Or Already Processed!\n";
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
}
PauseScreen();
}

void finance_menu(){
while(true){
ClearScreen();
cout << "\n===Finance Menu===\n";
cout << "1. Monthly Revenue Report\n";
cout << "2. Refund Loss Report\n";
cout << "3. Net Profit Report\n";
cout << "4. View Refunds\n";
cout << "5. Approve Refund\n";
cout << "6. Cancel Refund\n";
cout << "7. Back\n";
cout << "Enter Choice: ";
int ch = getValidInt(1, 7);
if(ch == 1) monthly_revenue();
else if(ch == 2) refund_loss();
else if(ch == 3) net_profit();
else if(ch == 4) finance_view_refunds();
else if(ch == 5) finance_approve_refund();
else if(ch == 6) finance_cancel_refund();
else if(ch == 7) return;
}
}

void finance_login(){
string u, p;
ClearScreen();
while(true){
ifstream check(FINANCE_FILE);
if(!check.is_open()){
cout << "Finance File Not Found!\n";
cout << "Creating Default Finance Account...\n";
try{
ofstream create(FINANCE_FILE);
if(!create.is_open()) throw runtime_error("Failed To Create Finance File");
create << "finance Finance@1234" << endl;
create.close();
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
PauseScreen();
return;
}
cout << "Default Username : finance\n";
cout << "Default Password : Finance@1234\n";
PauseScreen();
ClearScreen();
check.open(FINANCE_FILE);
if(!check.is_open()){
PauseScreen();
return;
}
}

cout << "\n===Finance Login===\n";
cout << "Enter Finance Username: ";
cin >> loguser;
if(loguser.empty()){
cout << "Username Cannot Be Empty!\n";
check.close();
continue;
}

cout << "Enter Finance Password: ";
cin >> logpass;
if(logpass.empty()){
cout << "Password Cannot Be Empty!\n";
check.close();
continue;
}

bool exist = false;
while(check >> u >> p){
if(loguser == u && logpass == p){
exist = true;
break;
}
}
check.close();

if(exist){
cout << "Login Successful\n";
logActivity("Finance Login");
finance_menu();
break;
}
else{
cout << "Login Failed!\n";
cout << "1. Try Again\n2. Back To Menu\n";
cout << "Enter Choice: ";
int ch = getValidInt(1, 2);
if(ch != 1){
return;
}
}
}
}

void admin_login(){
string u, p;
ClearScreen();
while(true){
bool exist = false;
ifstream check(ADMIN_FILE);
if(!check.is_open()){
cout << "Admin file nahi mili!\n";
PauseScreen();
return;
}

cout << "Enter Admin Username: ";
cin >> loguser;
if(loguser.empty()){
cout << "Username Cannot Be Empty!\n";
continue;
}

cout << "Enter Admin Password: ";
cin >> logpass;
if(logpass.empty()){
cout << "Password Cannot Be Empty!\n";
continue;
}

while(check >> u >> p){
if(loguser == u && logpass == p){
exist = true;
break;
}
}
check.close();
if(exist){
cout << "Login Successful\n";
admin_menu();
break;
}
else{
cout << "Login Failed!\n";
cout << "1. Try Again\n2. Back To Menu\n";
cout << "Enter Choice: ";
int ch = getValidInt(1, 2);
if(ch != 1){
return;
}
}
}
}

void admin_menu(){
while(true){
ClearScreen();
cout << "\n===Admin Menu===\n";
cout << "1. Add Bus\n";
cout << "2. Edit Bus\n";
cout << "3. Remove Bus\n";
cout << "4. Remove Account\n";
cout << "5. Change Password\n";
cout << "6. View Users\n";
cout << "7. View Bookings\n";
cout << "8. View Logs\n";
cout << "9. Destroy Bus\n";
cout << "10. View Refunds\n";
cout << "11. Approve Refund\n";
cout << "12. Cancel Refund\n";
cout << "13. Back To Login\n";
cout << "Enter Choice: ";
int ch = getValidInt(1, 13);
if(ch == 1) add_bus();
else if(ch == 2) edit_bus();
else if(ch == 3) remove_bus();
else if(ch == 4) remove_account();
else if(ch == 5) change_password();
else if(ch == 6) view_users();
else if(ch == 7) view_bookings();
else if(ch == 8) view_logs();
else if(ch == 9) destroy_bus();
else if(ch == 10) view_refunds();
else if(ch == 11) approve_refund();
else if(ch == 12) cancel_refund();
else if(ch == 13) return;
}
}

bool MazbootPassword(const string& pass){
if(pass.length() < 8) return false;
bool hasUpper = false, hasDigit = false, hasSpecial = false;
for(int i = 0; i < pass.length(); i++){
char c = pass[i];
if(isupper(c)) hasUpper = true;
if(isdigit(c)) hasDigit = true;
if(!isalnum(c)) hasSpecial = true;
}
return hasUpper && hasDigit && hasSpecial;
}

string getValidPassword(){
string pass;
cout << "Enter Password: ";
cin >> pass;

if(pass.empty()){
cout << "Password Cannot Be Empty!\n";
return getValidPassword();
}

if(!MazbootPassword(pass)){
cout << "\n===Weak Password!===\n";
cout << "- Atleast 8 characters\n";
cout << "- One Capital Letter\n";
cout << "- One Number\n";
cout << "- One Special Character\n";
cout << "Example: Ali@1234\n\n";
return getValidPassword();
}
return pass;
}

void register_(){
User newUser;
string u, p;
bool exist = false;
ClearScreen();
cout << "\n===Register Account===\n";
while(true){
exist = false;

while(true){
cout << "Create Your User Name: ";
cin >> newUser.username;
if(!isValidName(newUser.username)){
cout << "Invalid Username! Use Letters, Digits Or Underscore Only.\n";
continue;
}
break;
}

newUser.password = getValidPassword();

try{
ifstream check(ACCOUNTS_FILE);
if(!check.is_open()) throw runtime_error("Failed To Open Accounts File For Reading");
while(check >> u >> p){
if(u == newUser.username){
exist = true;
break;
}
}
check.close();
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
PauseScreen();
return;
}

if(exist){
cout << "Username Already Exists!\n";
}
else{
regusername = newUser.username;
userpassword = newUser.password;
try{
ofstream enter(ACCOUNTS_FILE, ios::app);
if(!enter.is_open()) throw runtime_error("Failed To Open Accounts File For Writing");
enter << newUser.username << " "
<< newUser.password << endl;
enter.close();
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
PauseScreen();
return;
}
cout << "Account Created!\n";
logActivity("New Account: " + newUser.username);
break;
}
}
cout << "Press 1 To Login: ";
int n = getValidInt(1, 2);
if(n == 1) log_in();
else return;
}

void log_in(){
ClearScreen();
cout << "\n===Log In===\n";
int attempts = 0;

while(attempts < MAX_ATTEMPTS){
attempts++;

cout << "Enter Your User Name: ";
cin >> loguser;
if(loguser.empty()){
cout << "Username Cannot Be Empty!\n";
attempts--;
continue;
}

cout << "Enter Your Password: ";
cin >> logpass;
if(logpass.empty()){
cout << "Password Cannot Be Empty!\n";
attempts--;
continue;
}

bool found = false;
try{
ifstream file(ACCOUNTS_FILE);
if(!file.is_open()) throw runtime_error("Failed To Open Accounts File For Reading");
string u, p;
while(file >> u >> p){
if(loguser == u && logpass == p){
found = true;
break;
}
}
file.close();
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
PauseScreen();
return;
}

if(found){
cout << " Login Successful!\n";
logActivity("Logged In");
attempts = 0;
realmenu();
return;
}
else{
cout << " Login Failed!\n";
if(attempts == MAX_ATTEMPTS){
cout << "================================\n";
cout << " Account Locked!\n";
cout << " Too Many Wrong Attempts!\n";
cout << "================================\n";
logActivity("Account Locked - Too Many Attempts");
PauseScreen();
return;
}
cout << "Attempts Remaining: "
<< MAX_ATTEMPTS - attempts << "\n";
cout << "1. Try Again\n";
cout << "2. Back To Menu\n";
cout << "Enter Choice: ";
int ch = getValidInt(1, 2);
if(ch == 1){
continue;
}
else{
return;
}
}
}
}

void Menu(){
string choice;
while(true){
ClearScreen();
cout << "\n===Welcome To Bus Reservation System===\n";
cout << "What would you like to do" << endl;
cout << "0. Admin Log_in \n";
cout << "1. Register Your Account \n";
cout << "2. Log_in Your Account \n";
cout << "3. Finance Log_in \n";
cout << "4. Exit \n";
cout << "Enter Your Choice: ";
cin >> choice;
if(choice == "1"){
register_();
}
else if(choice == "0"){
admin_login();
}
else if(choice == "2"){
log_in();
}
else if(choice == "3"){
finance_login();
}
else if(choice == "4"){
break;
}
else{
cout << "INVALID RESPONSE\n";
PauseScreen();
}
}
}

string generateToken(const string& busNo,
const string& seatNo,
const string& username){
string token = "TKN-" + username + "-" + busNo + "-" + seatNo;
return token;
}

void realmenu(){
string opt;
do{
ClearScreen();
cout << "\n===Bus Management===\n";
cout << "1. Book Seat\n";
cout << "2. Check Route Timing\n";
cout << "3. Bus Details\n";
cout << "4. Check My Booking\n";
cout << "5. Cancel Booking\n";
cout << "6. My All Bookings\n";
cout << "7. Search Route\n";
cout << "8. Change My Password\n";
cout << "9. Review\n";
cout << "10. My Refunds\n";
cout << "11. Back To Login\n";
cout << "Enter Choice: ";
cin >> opt;
if(opt == "1") book_seat();
else if(opt == "2") route_time();
else if(opt == "3") bus_detail();
else if(opt == "4") check_booking();
else if(opt == "5") cancel_booking();
else if(opt == "6") my_bookings();
else if(opt == "7") search_route();
else if(opt == "8") user_change_password();
else if(opt == "9") review_();
else if(opt == "11") return;
else if(opt == "10") my_refunds();
else{
cout << "INVALID RESPONSE\n";
PauseScreen();
}
}while(opt != "11");
}

void book_seat(){
ClearScreen();
bus_detail();

Bus selectedBus;
Booking newBooking;

while(true){
cout << "\nEnter Bus Number: ";
cin >> selectedBus.busNo;
if(!isValidBusNo(selectedBus.busNo)){
cout << "Invalid Bus Number Format!\n";
continue;
}
break;
}

int totalSeats = 0;
int price = 0;
bool busFound = false;

try{
ifstream busFile(BUSES_FILE);
if(!busFile.is_open()) throw runtime_error("Failed To Open Buses File For Reading");
string b, t, s, r, pr, dep, arr;

while(busFile >> b >> t >> s >> r >> pr >> dep >> arr){
if(b == selectedBus.busNo){
selectedBus.type = t;
selectedBus.route = r;
selectedBus.price = pr;
selectedBus.departure = dep;
selectedBus.arrival = arr;
try{
totalSeats = stringToInt(s);
price = stringToInt(pr);
}
catch(runtime_error& e){
busFile.close();
throw runtime_error("Invalid Bus Data: " + string(e.what()));
}
selectedBus.seats = totalSeats;
busFound = true;
break;
}
}
busFile.close();
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
PauseScreen();
return;
}

if(!busFound){
cout << "Bus Not Found!\n";
PauseScreen();
return;
}

cout << "\n===Bus Information===\n";
cout << "Route : " << selectedBus.route << endl;
cout << "Type : " << selectedBus.type << endl;
cout << "Departure : " << selectedBus.departure << endl;
cout << "Arrival : " << selectedBus.arrival << endl;
cout << "Price : Rs." << price << endl;
cout << "Total Seats: " << totalSeats << endl;

int bookedSeatNums[MAX_SEATS];
int bookedCount = 0;

try{
ifstream seatFile(SEATS_FILE);
if(!seatFile.is_open()) throw runtime_error("Failed To Open Seats File For Reading");
string su, sb, ss, st;
while(seatFile >> su >> sb >> ss >> st){
if(sb == selectedBus.busNo){
try{
bookedSeatNums[bookedCount] = stringToInt(ss);
}
catch(runtime_error&){
bookedSeatNums[bookedCount] = 0;
}
bookedCount++;
}
}
seatFile.close();
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
PauseScreen();
return;
}

int availableSeats = totalSeats - bookedCount;
if(availableSeats <= 0){
cout << "\n=============================\n";
cout << " Bus Is Fully Booked!\n";
cout << " No Seats Available!\n";
cout << "=============================\n";
PauseScreen();
return;
}

cout << "\n===Seat Map===\n";
cout << "[ ] = Available [X] = Booked\n";
cout << "Available Seats: " << availableSeats << "/" << totalSeats << "\n\n";

for(int i = 1; i <= totalSeats; i++){
bool isBooked = false;
for(int j = 0; j < bookedCount; j++){
if(bookedSeatNums[j] == i){
isBooked = true;
break;
}
}
if(isBooked){
if(i < 10) cout << "[X0" << i << "] ";
else cout << "[X" << i << "] ";
}
else{
if(i < 10) cout << "[ 0" << i << "] ";
else cout << "[ " << i << "] ";
}
if(i % 4 == 0) cout << endl;
}
cout << "\n";

newBooking.username = loguser;
newBooking.busNo = selectedBus.busNo;

cout << "\nEnter Seat Number (1-" << totalSeats << "): ";
int chosenSeat = getValidSeatNo(totalSeats, bookedSeatNums, bookedCount);

newBooking.seatNo = intToString(chosenSeat);
newBooking.token = "TKN-" + loguser + "-" + selectedBus.busNo + "-" + newBooking.seatNo;

try{
ofstream booking(SEATS_FILE, ios::app);
if(!booking.is_open()) throw runtime_error("Failed To Open Seats File For Writing");
booking << newBooking.username << " "
<< newBooking.busNo << " "
<< newBooking.seatNo << " "
<< newBooking.token << endl;
booking.close();
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
PauseScreen();
return;
}

cout << "\n=============================\n";
cout << " BOOKING CONFIRMED! \n";
cout << "=============================\n";
cout << "Token : " << newBooking.token << endl;
cout << "User : " << newBooking.username << endl;
cout << "Bus : " << newBooking.busNo << endl;
cout << "Route : " << selectedBus.route << endl;
cout << "Seat : " << newBooking.seatNo << endl;
cout << "Departure : " << selectedBus.departure << endl;
cout << "Arrival : " << selectedBus.arrival << endl;
cout << "Price : Rs." << price << endl;
cout << "=============================\n";
cout << " SAVE YOUR TICKET PLEASE \n";
cout << "=============================\n";

logActivity("Seat Booked - Bus:" + newBooking.busNo +
" Seat:" + newBooking.seatNo);
PauseScreen();
}

void route_time(){
ClearScreen();
Bus* buses = new Bus[MAX_BUSES];

int count = 0;
try{
ifstream file(BUSES_FILE);
if(!file.is_open()) throw runtime_error("Failed To Open Buses File For Reading");
string b, t, s, r, p, dep, arr;
while(file >> b >> t >> s >> r >> p >> dep >> arr){
buses[count].busNo = b;
buses[count].type = t;
buses[count].route = r;
buses[count].price = p;
buses[count].departure = dep;
buses[count].arrival = arr;
count++;
}
file.close();
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
delete[] buses;
PauseScreen();
return;
}

if(count == 0){
cout << "No Routes Available!\n";
delete[] buses;
PauseScreen();
return;
}
cout << "\n===Sort Routes By===\n";
cout << "1. Price (Low to High)\n";
cout << "2. Route (A to Z)\n";
cout << "3. No Sorting\n";
cout << "Enter Choice: ";
int sortChoice = getValidInt(1, 3);

if(sortChoice == 1 || sortChoice == 2){
sortBuses(buses, count, sortChoice);
cout << "\n[Sorted Successfully!]\n";
}
cout << "\n===Route & Timing===\n";
cout << "No. Route BusNo Departure Arrival Price\n";
cout << "-------------------------------------------------------------------\n";

for(int i = 0; i < count; i++){
cout << i+1 << ". "
<< buses[i].route << " "
<< buses[i].busNo << " "
<< buses[i].departure << " "
<< buses[i].arrival << " Rs."
<< buses[i].price << endl;
}
delete[] buses;
PauseScreen();
}

void bus_detail(){
ClearScreen();
Bus* buses = new Bus[MAX_BUSES];
int count = 0;
try{
ifstream file(BUSES_FILE);
if(!file.is_open()) throw runtime_error("Failed To Open Buses File For Reading");
string b, t, s, r, p, dep, arr;
while(file >> b >> t >> s >> r >> p >> dep >> arr){
buses[count].busNo = b;
buses[count].type = t;
try{
buses[count].seats = stringToInt(s);
}
catch(runtime_error&){
buses[count].seats = 0;
}
buses[count].route = r;
buses[count].price = p;
buses[count].departure = dep;
buses[count].arrival = arr;
count++;
}
file.close();
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
delete[] buses;
PauseScreen();
return;
}

if(count == 0){
cout << "No Buses Available!\n";
delete[] buses;
PauseScreen();
return;
}
cout << "\n===Sort Buses By===\n";
cout << "1. Price (Low to High)\n";
cout << "2. Route (A to Z)\n";
cout << "3. Bus Number\n";
cout << "4. No Sorting\n";
cout << "Enter Choice: ";
int sortChoice = getValidInt(1, 4);
if(sortChoice >= 1 && sortChoice <= 3){
sortBuses(buses, count, sortChoice);
cout << "\n[Sorted Successfully!]\n";
}
cout << "\n===Bus Details===\n";
cout << "No. BusNo Type Seats Route Price Depart Arrive\n";
cout << "-------------------------------------------------------------------------------\n";

for(int i = 0; i < count; i++){
cout << i+1 << ". "
<< buses[i].busNo << " "
<< buses[i].type << " "
<< buses[i].seats << " "
<< buses[i].route << " Rs."
<< buses[i].price << " "
<< buses[i].departure << " "
<< buses[i].arrival << endl;
}
delete[] buses;
PauseScreen();
}

void review_(){
string opt2;
do{
ClearScreen();
cout << "\n===Review System===\n";
cout << "1. Enter Review\n";
cout << "2. Other People Review\n";
cout << "3. Back to menu\n";
cout << "Enter choice: ";
cin >> opt2;
if(opt2 == "1"){
ent_rev();
}
else if(opt2 == "2"){
oth_rev();
}
else if(opt2 == "3"){
return;
}
else{
cout << "Invalid Input\n";
PauseScreen();
}
}while(opt2 != "3");
}

void ent_rev(){
ClearScreen();
cin.ignore();
string rev;
while(true){
cout << "Enter your review: ";
getline(cin, rev);
if(rev.empty()){
cout << "Review Cannot Be Empty!\n";
continue;
}
if(rev.length() > 300){
cout << "Review Too Long! Maximum 300 Characters.\n";
continue;
}
break;
}
try{
ofstream review(REVIEWS_FILE, ios::app);
if(!review.is_open()) throw runtime_error("Failed To Open Reviews File For Writing");
review << loguser << " : " << rev << endl;
review.close();
cout << "Thank you for your review!\n";
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
}
PauseScreen();
}

void oth_rev(){
ClearScreen();
cout << "\n===Other People's Reviews===\n";
try{
ifstream review(REVIEWS_FILE);
if(!review.is_open()) throw runtime_error("Failed To Open Reviews File For Reading");
string line;
int count = 1;
bool empty = true;
while(getline(review, line)){
cout << count << " . " << line << endl;
count++;
empty = false;
}
review.close();
if(empty){
cout << "No reviews yet\n";
}
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
}
PauseScreen();
}

void check_booking(){
ClearScreen();
cout << "\n===Check Booking===\n";
cout << "Enter Your Token: ";
string token = getValidToken();

bool found = false;
try{
ifstream file(SEATS_FILE);
if(!file.is_open()) throw runtime_error("Failed To Open Seats File For Reading");
string u, b, s, t;

while(file >> u >> b >> s >> t){
if(t == token){
found = true;
string dep, arr, route, price;
try{
ifstream busFile(BUSES_FILE);
if(!busFile.is_open()) throw runtime_error("Failed To Open Buses File For Reading");
string bb, tt, ss, rr, pp, dd, aa;

while(busFile >> bb >> tt >> ss >> rr >> pp >> dd >> aa){
if(bb == b){
dep = dd;
arr = aa;
route = rr;
price = pp;
break;
}
}
busFile.close();
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
}

cout << "\n=============================\n";
cout << " BOOKING DETAILS \n";
cout << "=============================\n";
cout << "Token : " << t << endl;
cout << "User : " << u << endl;
cout << "Bus : " << b << endl;
cout << "Route : " << route << endl;
cout << "Seat : " << s << endl;
cout << "Departure : " << dep << endl;
cout << "Arrival : " << arr << endl;
cout << "Price : Rs." << price << endl;
cout << "=============================\n";
break;
}
}
file.close();
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
}
if(!found){
cout << "Token Not Found!\n";
}
PauseScreen();
}

void cancel_booking(){
ClearScreen();
cout << "\n===Cancel Booking===\n";
cout << "Enter Your Token To Cancel: ";
string token = getValidToken();

bool found = false;
try{
ifstream fin(SEATS_FILE);
if(!fin.is_open()) throw runtime_error("Failed To Open Seats File For Reading");
ofstream fout("temp.txt");
if(!fout.is_open()){
fin.close();
throw runtime_error("Failed To Open Temp File For Writing");
}
string u, b, s, t;

while(fin >> u >> b >> s >> t){
if(t == token && u == loguser){
found = true;
cout << "Booking Cancelled!\n";
cout << "Seat " << s << " Is Now Free!\n";
}
else{
fout << u << " " << b << " "
<< s << " " << t << endl;
}
}
fin.close();
fout.close();

if(remove(SEATS_FILE) != 0) throw runtime_error("Failed To Remove Old Seats File");
if(rename("temp.txt", SEATS_FILE) != 0) throw runtime_error("Failed To Rename Temp File");
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
PauseScreen();
return;
}

if(!found){
cout << "Token Not Found Or Does Not Belong To You!\n";
}
else{
logActivity("Booking Cancelled - Token:" + token);
}
PauseScreen();
}

void user_change_password(){
ClearScreen();
cout << "\n===Change My Password===\n";
string oldPass, newPass;
cout << "Enter Current Password: ";
cin >> oldPass;

if(oldPass.empty()){
cout << "Password Cannot Be Empty!\n";
PauseScreen();
return;
}

bool verified = false;
try{
ifstream check(ACCOUNTS_FILE);
if(!check.is_open()) throw runtime_error("Failed To Open Accounts File For Reading");
string u, p;
while(check >> u >> p){
if(u == loguser && p == oldPass){
verified = true;
break;
}
}
check.close();
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
PauseScreen();
return;
}

if(!verified){
cout << "Wrong Current Password!\n";
PauseScreen();
return;
}
while(true){
cout << "Enter New Password: ";
cin >> newPass;
if(newPass.empty()){
cout << "Password Cannot Be Empty!\n";
continue;
}
if(!MazbootPassword(newPass)){
cout << "Weak Password! Try Again.\n";
continue;
}
break;
}
try{
ifstream fin(ACCOUNTS_FILE);
if(!fin.is_open()) throw runtime_error("Failed To Open Accounts File For Reading");
ofstream fout("temp.txt");
if(!fout.is_open()){
fin.close();
throw runtime_error("Failed To Open Temp File For Writing");
}
string u, p;
while(fin >> u >> p){
if(u == loguser)
fout << u << " " << newPass << endl;
else
fout << u << " " << p << endl;
}
fin.close();
fout.close();

if(remove(ACCOUNTS_FILE) != 0) throw runtime_error("Failed To Remove Old Accounts File");
if(rename("temp.txt", ACCOUNTS_FILE) != 0) throw runtime_error("Failed To Rename Temp File");
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
PauseScreen();
return;
}
cout << "Password Changed Successfully!\n";
logActivity("Password Changed");
PauseScreen();
}

void search_route(){
ClearScreen();
string searchRoute;
cout << "\n===Search Route===\n";

while(true){
cout << "Enter City Name (e.g Karachi): ";
cin >> searchRoute;
if(searchRoute.empty()){
cout << "City Name Cannot Be Empty!\n";
continue;
}
bool validCity = true;
for(int i = 0; i < searchRoute.length(); i++){
if(!isalpha(searchRoute[i])){
validCity = false;
break;
}
}
if(!validCity){
cout << "Invalid City Name! Use Letters Only.\n";
continue;
}
break;
}

bool found = false;
try{
ifstream file(BUSES_FILE);
if(!file.is_open()) throw runtime_error("Failed To Open Buses File For Reading");
string b, t, s, r, p, dep, arr;
int count = 1;
cout << "\n===Search Results===\n";
while(file >> b >> t >> s >> r >> p >> dep >> arr){
string lowerR = r, lowerSearch = searchRoute;
for(int i = 0; i < lowerR.length(); i++)
lowerR[i] = tolower(lowerR[i]);
for(int i = 0; i < lowerSearch.length(); i++)
lowerSearch[i] = tolower(lowerSearch[i]);
if(lowerR.find(lowerSearch) != string::npos){
cout << count << ". Bus: " << b
<< " | Route: " << r
<< " | Depart: " << dep
<< " | Rs." << p << endl;
count++;
found = true;
}
}
file.close();
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
PauseScreen();
return;
}
if(!found) cout << "No Routes Found For: " << searchRoute << "\n";
PauseScreen();
}

void my_bookings(){
ClearScreen();
cout << "\n===My Bookings===\n";
bool found = false;
try{
ifstream file(SEATS_FILE);
if(!file.is_open()) throw runtime_error("Failed To Open Seats File For Reading");
string u, b, s, t;
int count = 1;
while(file >> u >> b >> s >> t){
if(u == loguser){
string dep, arr, route, price;
try{
ifstream busFile(BUSES_FILE);
if(!busFile.is_open()) throw runtime_error("Failed To Open Buses File For Reading");
string bb, tt, ss, rr, pp, dd, aa;
while(busFile >> bb >> tt >> ss >> rr >> pp >> dd >> aa){
if(bb == b){
dep = dd;
arr = aa;
route = rr;
price = pp;
break;
}
}
busFile.close();
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
}
cout << "================================\n";
cout << count << ". Token : " << t << endl;
cout << " Bus : " << b << endl;
cout << " Route : " << route << endl;
cout << " Seat : " << s << endl;
cout << " Depart: " << dep << endl;
cout << " Price : Rs." << price << endl;
count++;
found = true;
}
}
file.close();
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
PauseScreen();
return;
}
if(!found) cout << "No Bookings Found!\n";
PauseScreen();
}

void edit_bus(){
string busNo;
ClearScreen();
cout << "\n===Edit Bus Details===\n";

while(true){
cout << "Enter Bus Number To Edit: ";
cin >> busNo;
if(!isValidBusNo(busNo)){
cout << "Invalid Bus Number Format!\n";
continue;
}
break;
}

try{
ifstream fin(BUSES_FILE);
if(!fin.is_open()) throw runtime_error("Failed To Open Buses File For Reading");
ofstream fout("temp.txt");
if(!fout.is_open()){
fin.close();
throw runtime_error("Failed To Open Temp File For Writing");
}
string b, t, s, r, p, dep, arr;
bool found = false;
while(fin >> b >> t >> s >> r >> p >> dep >> arr){
if(b == busNo){
found = true;
cout << "\nCurrent Details:\n";
cout << "1. Type : " << t << "\n";
cout << "2. Seats : " << s << "\n";
cout << "3. Route : " << r << "\n";
cout << "4. Price : " << p << "\n";
cout << "5. Departure : " << dep << "\n";
cout << "6. Arrival : " << arr << "\n";
cout << "\nWhat To Edit (1-6): ";
int ch = getValidInt(1, 6);

if(ch == 1){
while(true){
cout << "New Type (AC/NonAC): ";
cin >> t;
if(t != "AC" && t != "NonAC"){
cout << "Invalid Type!\n";
continue;
}
break;
}
}
else if(ch == 2){
while(true){
cout << "New Seats (1-100): ";
cin >> s;
if(!isAllDigits(s)){
cout << "Invalid! Numbers Only.\n";
continue;
}
int sv;
try{
sv = stringToInt(s);
}
catch(runtime_error&){
cout << "Invalid Seat Value!\n";
continue;
}
if(sv < 1 || sv > 100){
cout << "Seats Must Be 1-100.\n";
continue;
}
break;
}
}
else if(ch == 3){
while(true){
cout << "New Route (e.g Karachi-Lahore): ";
cin >> r;
if(!isValidRoute(r)){
cout << "Invalid Route Format!\n";
continue;
}
break;
}
}
else if(ch == 4){
while(true){
cout << "New Price: ";
cin >> p;
if(!isAllDigits(p)){
cout << "Invalid Price! Numbers Only.\n";
continue;
}
break;
}
}
else if(ch == 5){
while(true){
cout << "New Departure: ";
cin >> dep;
if(!isValidTime(dep)){
cout << "Invalid Time Format!\n";
continue;
}
break;
}
}
else if(ch == 6){
while(true){
cout << "New Arrival: ";
cin >> arr;
if(!isValidTime(arr)){
cout << "Invalid Time Format!\n";
continue;
}
break;
}
}
fout << b << " " << t << " " << s << " " << r
<< " " << p << " " << dep << " " << arr << endl;
cout << "Bus Updated!\n";
logActivity("Bus Edited: " + busNo);
}
else{
fout << b << " " << t << " " << s << " " << r
<< " " << p << " " << dep << " " << arr << endl;
}
}
fin.close();
fout.close();

if(remove(BUSES_FILE) != 0) throw runtime_error("Failed To Remove Old Buses File");
if(rename("temp.txt", BUSES_FILE) != 0) throw runtime_error("Failed To Rename Temp File");

if(!found) cout << "Bus Not Found!\n";
}
catch(runtime_error& e){
cout << "[File Error] " << e.what() << "\n";
}
PauseScreen();
}
