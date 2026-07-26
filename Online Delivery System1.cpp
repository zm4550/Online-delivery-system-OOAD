#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <map>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cstdlib>

using namespace std;

// =============================
// FORWARD DECLARATIONS
// =============================
class User;
class Customer;
class Shopkeeper;
class Shop;
class Item;
class DeliveryBoy;
class Transaction;
class Moderator;

// =============================
// GLOBAL VARIABLE DECLARATIONS (extern)
// =============================
extern vector<User*> allUsers;
extern vector<Shop*> allShops;
extern vector<Item*> allItems;
extern vector<Transaction*> allTransactions;
extern Moderator* systemAdmin;
extern User* currentUser;

// =============================
// HELPER FUNCTIONS AND STRUCTURES
// =============================
struct Date {
    int day, month, year;
    Date(int d = 1, int m = 1, int y = 2024) : day(d), month(m), year(y) {}
    string toString() const {
        stringstream ss;
        ss << day << "/" << month << "/" << year;
        return ss.str();
    }
    
    static Date getToday() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        return Date(ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year);
    }
};

struct DeliverySlot {
    string time;
    Date date;
    DeliverySlot(string t = "10:00", Date d = Date()) : time(t), date(d) {}
    string toString() const {
        return date.toString() + " " + time;
    }
};

// Helper function to convert int to string
string intToString(int num) {
    stringstream ss;
    ss << num;
    return ss.str();
}

// Helper to clear screen (platform specific)
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Helper to pause execution
void pressEnterToContinue() {
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

// =============================
// USER BASE CLASS
// =============================
class User {
protected:
    string Name;
    string EmailID;
    string Password;
    string Contact_Number;
    string user_type;
    static int userCount;

public:
    User(string name = "", string email = "", string password = "", 
         string contact = "", string type = "User")
        : Name(name), EmailID(email), Password(password), 
          Contact_Number(contact), user_type(type) {
        userCount++;
    }

    virtual ~User() {}

    bool authenticate(string pass) {
        return Password == pass;
    }

    string getName() const { return Name; }
    string getEmail() const { return EmailID; }
    string getContact() const { return Contact_Number; }
    string getUserType() const { return user_type; }
    string getPassword() const { return Password; }

    virtual void display() const {
        cout << "\n=== User Profile ===" << endl;
        cout << "Name: " << Name 
             << "\nEmail: " << EmailID
             << "\nContact: " << Contact_Number
             << "\nType: " << user_type << endl;
    }
    
    virtual void showMenu() = 0;

    static int getUserCount() { return userCount; }
    
    static User* login();
    
    static void registerUser();
};

int User::userCount = 0;

// =============================
// ITEM CLASS
// =============================
class Item {
private:
    static int itemCount;
    
public:
    int ID;
    vector<string> Keywords;
    double Price;
    int Quantity;
    string Description;
    string name;
    Shop* shop;  // Which shop this item belongs to

    Item(string n = "", double p = 0, int q = 0, string desc = "", Shop* s = NULL);
    
    void addKeyword(string kw);
    
    bool Match_Keyword(const string& keyword) const;
    
    void display() const;
    
    static int getItemCount() { return itemCount; }
    
    static Item* searchItem(int id);
};

int Item::itemCount = 0;

// =============================
// SHOP CLASS
// =============================
class Shop {
private:
    static int shopCount;
    
public:
    int shopId;
    string Shop_Name;
    string Timings;
    string Description;
    Shopkeeper* Owner;
    vector<Item*> List_of_items;

    Shop(string name = "", string timings = "9-5", string desc = "", 
         Shopkeeper* owner = NULL);
    
    void Search_Item(const string& keyword) const;
    
    void addItem(Item* item);
    
    void displayItems() const;
    
    Item* getItemById(int id);
};

int Shop::shopCount = 0;

// =============================
// CUSTOMER CLASS
// =============================
class Customer : public User {
private:
    double Credit_Balance;
    double Wallet_Balance;
    vector<Item*> Cart;
    vector<Transaction*> Transaction_History;
    bool Defaulter;

public:
    Customer(string name = "", string email = "", string password = "", 
             string contact = "");
    
    void Add_moneyToWallet();
    
    void Add_Cart();
    
    void Remove_Cart();
    
    void Cancel_Order();
    
    void Select_Shop();
    
    void displayCart() const;
    
    void placeOrder();
    
    void viewOrderHistory();
    
    double getWalletBalance() const { return Wallet_Balance; }
    double getCreditBalance() const { return Credit_Balance; }
    bool isDefaulter() const { return Defaulter; }
    const vector<Item*>& getCart() const { return Cart; }

    void setDefaulter(bool status) { Defaulter = status; }
    void setWalletBalance(double amount) { Wallet_Balance = amount; }
    void setCreditBalance(double amount) { Credit_Balance = amount; }
    void clearCart() { Cart.clear(); }
    void addTransaction(Transaction* t) { Transaction_History.push_back(t); }
    
    void showMenu();
};

// =============================
// SHOPKEEPER CLASS
// =============================
class Shopkeeper : public User {
private:
    Shop* myShop;
    string Account_No;
    double Pending_Amount;
    vector<Transaction*> Transaction_History;
    vector<string> Holiday_List;

public:
    Shopkeeper(string name = "", string email = "", string password = "", 
               string contact = "", string accNo = "");
    
    void Shop_Registration();
    
    void Update_Records();
    
    void add_new_item();
    
    void add_new_holiday();
    
    void displayHolidays() const;
    
    void setPendingAmount(double amount) { Pending_Amount = amount; }
    double getPendingAmount() const { return Pending_Amount; }
    string getAccountNo() const { return Account_No; }
    Shop* getShop() const { return myShop; }
    void setShop(Shop* shop) { myShop = shop; }
    
    void viewShopItems();
    
    void showMenu();
};

// =============================
// DELIVERY BOY CLASS
// =============================
class DeliveryBoy : public User {
private:
    vector<Transaction*> Transaction_History;
    bool Availability;

public:
    DeliveryBoy(string name = "", string email = "", string password = "", 
                string contact = "");
    
    void addTransaction(Transaction* t);
    
    void viewAssignedDeliveries();
    
    void toggleAvailability();
    
    bool isAvailable() const { return Availability; }
    void setAvailability(bool avail) { Availability = avail; }
    
    void showMenu();
};

// =============================
// TRANSACTION CLASS
// =============================
class Transaction {
private:
    static int transactionCount;
    
public:
    string Transaction_ID;
    Customer* customer;
    Shop* shop;
    string Payment_Type;
    DeliveryBoy* Delivery_Boy;
    vector<Item*> items;
    string Payment_status;
    Date date;
    string delivery_place;
    DeliverySlot Delivery_slot;
    double totalAmount;

    Transaction(Customer* cust = NULL, Shop* sh = NULL, 
                string payType = "Cash");
    
    bool is_possible() const;
    
    void Get_details() const;
    
    void CheckOut();
    
    void setDeliveryBoy(DeliveryBoy* db) { Delivery_Boy = db; }
    void setDeliveryAddress(const string& addr) { delivery_place = addr; }
    void setDeliverySlot(const DeliverySlot& slot) { Delivery_slot = slot; }
    void addItem(const Item& item);
};

int Transaction::transactionCount = 0;

// =============================
// MODERATOR CLASS
// =============================
class Moderator : public User {
private:
    vector<DeliveryBoy*> Delivery_Boys;
    vector<Customer*> Customers;
    vector<Shopkeeper*> Shopkeepers;
    double Available_balance;
    vector<string> holiday_list;

public:
    Moderator(string name = "", string email = "", string password = "", 
              string contact = "");
    
    DeliveryBoy* Find_delivery_boy();
    
    void Assign_delivery_boy(Transaction* transaction);
    
    void Collect_credit();
    
    void pay_shopkeeper();
    
    void mark_defaulters();
    
    void add_new_holiday();
    
    void Generate_spreadsheet();
    
    void viewSystemStats();
    
    void addDeliveryBoy(DeliveryBoy* db) { Delivery_Boys.push_back(db); }
    void addCustomer(Customer* cust) { Customers.push_back(cust); }
    void addShopkeeper(Shopkeeper* sk) { Shopkeepers.push_back(sk); }
    
    double getAvailableBalance() const { return Available_balance; }
    
    void showMenu();
};

// =============================
// GLOBAL VARIABLE DEFINITIONS
// =============================
vector<User*> allUsers;
vector<Shop*> allShops;
vector<Item*> allItems;
vector<Transaction*> allTransactions;
Moderator* systemAdmin = NULL;
User* currentUser = NULL;

// =============================
// METHOD IMPLEMENTATIONS
// =============================

// User static methods
User* User::login() {
    string email, password;
    cout << "\n=== LOGIN ===" << endl;
    cout << "Email: ";
    cin >> email;
    cout << "Password: ";
    cin >> password;
    
    for (size_t i = 0; i < allUsers.size(); i++) {
        if (allUsers[i]->getEmail() == email && allUsers[i]->authenticate(password)) {
            cout << "Login successful! Welcome " << allUsers[i]->getName() << endl;
            return allUsers[i];
        }
    }
    cout << "Invalid email or password!" << endl;
    return NULL;
}

void User::registerUser() {
    cout << "\n=== REGISTER ===" << endl;
    cout << "Select User Type:" << endl;
    cout << "1. Customer" << endl;
    cout << "2. Shopkeeper" << endl;
    cout << "3. Delivery Boy" << endl;
    cout << "Choice: ";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    string name, email, password, contact, accountNo;
    string userTypeToRegister = ""; // Will store the user type being registered
    
    cout << "Name: ";
    getline(cin, name);
    
    // Determine user type first
    switch(choice) {
        case 1:
            userTypeToRegister = "Customer";
            break;
        case 2:
            userTypeToRegister = "Shopkeeper";
            break;
        case 3:
            userTypeToRegister = "DeliveryBoy";
            break;
        default:
            cout << "Invalid choice!" << endl;
            return;
    }
    
    // Check for unique email only within the same user type
    // Different user types (Customer, Shopkeeper, Rider) can share the same email
    bool emailExists = true;
    while (emailExists) {
        cout << "Email: ";
        getline(cin, email);
        emailExists = false;
        for (size_t i = 0; i < allUsers.size(); i++) {
            // Only check if email exists for users of the SAME type
            if (allUsers[i]->getEmail() == email && 
                allUsers[i]->getUserType() == userTypeToRegister) {
                cout << "Email already exists for a " << userTypeToRegister 
                     << "! Please use a different email." << endl;
                emailExists = true;
                break;
            }
        }
    }
    
    // Password can be the same for all users (no uniqueness check needed)
    cout << "Password: ";
    getline(cin, password);
    
    cout << "Contact Number: ";
    getline(cin, contact);
    
    User* newUser = NULL;
    
    switch(choice) {
        case 1:
            newUser = new Customer(name, email, password, contact);
            break;
        case 2:
            cout << "Account Number: ";
            getline(cin, accountNo);
            newUser = new Shopkeeper(name, email, password, contact, accountNo);
            break;
        case 3:
            newUser = new DeliveryBoy(name, email, password, contact);
            break;
        default:
            cout << "Invalid choice!" << endl;
            return;
    }
    
    allUsers.push_back(newUser);
    cout << "\nRegistration successful!" << endl;
    cout << "You are now registered as a " << newUser->getUserType() << endl;
}

// Item methods
Item::Item(string n, double p, int q, string desc, Shop* s) 
    : name(n), Price(p), Quantity(q), Description(desc), shop(s) {
    ID = ++itemCount;
    allItems.push_back(this);
}

void Item::addKeyword(string kw) {
    Keywords.push_back(kw);
}

bool Item::Match_Keyword(const string& keyword) const {
    for (size_t i = 0; i < Keywords.size(); i++) {
        if (Keywords[i].find(keyword) != string::npos) {
            return true;
        }
    }
    if (Description.find(keyword) != string::npos) {
        return true;
    }
    if (name.find(keyword) != string::npos) {
        return true;
    }
    return false;
}

void Item::display() const {
    cout << "\nID: " << ID << " | " << name 
         << " | Rs." << Price << " | Qty: " << Quantity 
         << "\nDesc: " << Description << endl;
    if (!Keywords.empty()) {
        cout << "Keywords: ";
        for (size_t i = 0; i < Keywords.size(); i++) {
            cout << Keywords[i] << " ";
        }
        cout << endl;
    }
    if (shop) {
        cout << "Shop: " << shop->Shop_Name << endl;
    }
    cout << "------------------------" << endl;
}

Item* Item::searchItem(int id) {
    for (size_t i = 0; i < allItems.size(); i++) {
        if (allItems[i]->ID == id) {
            return allItems[i];
        }
    }
    return NULL;
}

// Shop methods
Shop::Shop(string name, string timings, string desc, Shopkeeper* owner)
    : Shop_Name(name), Timings(timings), Description(desc), Owner(owner) {
    shopId = ++shopCount;
}

void Shop::Search_Item(const string& keyword) const {
    cout << "\nSearch results for '" << keyword << "':" << endl;
    bool found = false;
    for (size_t i = 0; i < List_of_items.size(); i++) {
        if (List_of_items[i]->Match_Keyword(keyword)) {
            List_of_items[i]->display();
            found = true;
        }
    }
    if (!found) {
        cout << "No items found matching '" << keyword << "'" << endl;
    }
}

void Shop::addItem(Item* item) {
    List_of_items.push_back(item);
}

void Shop::displayItems() const {
    cout << "\n=== " << Shop_Name << " ===" << endl;
    cout << "Timings: " << Timings << endl;
    cout << "Description: " << Description << endl;
    cout << "Owner: " << (Owner ? Owner->getName() : "None") << endl;
    cout << "\n--- Items (" << List_of_items.size() << ") ---" << endl;
    for (size_t i = 0; i < List_of_items.size(); i++) {
        List_of_items[i]->display();
    }
}

Item* Shop::getItemById(int id) {
    for (size_t i = 0; i < List_of_items.size(); i++) {
        if (List_of_items[i]->ID == id) return List_of_items[i];
    }
    return NULL;
}

// Customer methods
Customer::Customer(string name, string email, string password, string contact)
    : User(name, email, password, contact, "Customer"),
      Credit_Balance(0), Wallet_Balance(1000), Defaulter(false) {}

void Customer::Add_moneyToWallet() {
    double amount;
    cout << "Current Wallet Balance: Rs." << Wallet_Balance << endl;
    cout << "Enter amount to add: Rs.";
    cin >> amount;
    if (amount > 0) {
        Wallet_Balance += amount;
        cout << "Rs." << amount << " added to wallet. New balance: Rs." 
             << Wallet_Balance << endl;
    } else {
        cout << "Invalid amount!" << endl;
    }
}

void Customer::Add_Cart() {
    cout << "\n=== Available Items ===" << endl;
    for (size_t i = 0; i < allItems.size(); i++) {
        cout << i+1 << ". ";
        allItems[i]->display();
    }
    
    if (allItems.empty()) {
        cout << "No items available!" << endl;
        return;
    }
    
    int choice;
    cout << "Enter item number to add to cart (0 to cancel): ";
    cin >> choice;
    
    if (choice > 0 && choice <= (int)allItems.size()) {
        Cart.push_back(allItems[choice-1]);
        cout << allItems[choice-1]->name << " added to cart." << endl;
    }
}

void Customer::Remove_Cart() {
    if (Cart.empty()) {
        cout << "Cart is empty!" << endl;
        return;
    }
    
    displayCart();
    int itemId;
    cout << "Enter item ID to remove (0 to cancel): ";
    cin >> itemId;
    
    if (itemId == 0) return;
    
    for (size_t i = 0; i < Cart.size(); i++) {
        if (Cart[i]->ID == itemId) {
            Cart.erase(Cart.begin() + i);
            cout << "Item removed from cart." << endl;
            return;
        }
    }
    cout << "Item not found in cart." << endl;
}

void Customer::displayCart() const {
    if (Cart.empty()) {
        cout << "Cart is empty." << endl;
        return;
    }
    cout << "\n=== Your Cart ===" << endl;
    double total = 0;
    for (size_t i = 0; i < Cart.size(); i++) {
        cout << i+1 << ". ";
        Cart[i]->display();
        total += Cart[i]->Price;
    }
    cout << "Total: Rs." << total << endl;
    cout << "Wallet Balance: Rs." << Wallet_Balance << endl;
    cout << "Credit Balance: Rs." << Credit_Balance << endl;
}

void Customer::placeOrder() {
    if (Cart.empty()) {
        cout << "Cart is empty! Add items first." << endl;
        return;
    }
    
    displayCart();
    
    cout << "\n=== Place Order ===" << endl;
    cout << "Select Payment Method:" << endl;
    cout << "1. Wallet (Current balance: Rs." << Wallet_Balance << ")" << endl;
    cout << "2. Credit" << endl;
    cout << "3. Cash on Delivery" << endl;
    cout << "Choice: ";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    string paymentType;
    switch(choice) {
        case 1: paymentType = "Wallet"; break;
        case 2: paymentType = "Credit"; break;
        case 3: paymentType = "Cash"; break;
        default:
            cout << "Invalid choice!" << endl;
            return;
    }
    
    // Create transaction
    Transaction* txn = new Transaction(this, NULL, paymentType);
    
    // Add items from cart
    for (size_t i = 0; i < Cart.size(); i++) {
        txn->addItem(*Cart[i]);
    }
    
    // Get delivery address
    string address;
    cout << "Enter delivery address: ";
    getline(cin, address);
    txn->setDeliveryAddress(address);
    
    // Set delivery slot
    txn->setDeliverySlot(DeliverySlot("14:00", Date::getToday()));
    
    // Assign delivery boy
    if (systemAdmin) {
        systemAdmin->Assign_delivery_boy(txn);
    }
    
    // Checkout
    txn->CheckOut();
    
    if (txn->Payment_status == "Paid" || txn->Payment_status == "Credit" || txn->Payment_status == "Cash_Pending") {
        // Clear cart
        Cart.clear();
        Transaction_History.push_back(txn);
        allTransactions.push_back(txn);
        cout << "Order placed successfully!" << endl;
    }
}

void Customer::viewOrderHistory() {
    if (Transaction_History.empty()) {
        cout << "No orders yet." << endl;
        return;
    }
    
    cout << "\n=== Order History ===" << endl;
    for (size_t i = 0; i < Transaction_History.size(); i++) {
        Transaction_History[i]->Get_details();
        cout << endl;
    }
}

void Customer::Cancel_Order() {
    if (Transaction_History.empty()) {
        cout << "No orders to cancel." << endl;
        return;
    }
    
    cout << "\n=== Cancel Order ===" << endl;
    for (size_t i = 0; i < Transaction_History.size(); i++) {
        cout << i+1 << ". ";
        Transaction_History[i]->Get_details();
        cout << endl;
    }
    
    int choice;
    cout << "Select order to cancel (0 to cancel): ";
    cin >> choice;
    
    if (choice > 0 && choice <= (int)Transaction_History.size()) {
        // Refund if paid
        if (Transaction_History[choice-1]->Payment_status == "Paid") {
            Wallet_Balance += Transaction_History[choice-1]->totalAmount;
            cout << "Refund of Rs." << Transaction_History[choice-1]->totalAmount 
                 << " added to wallet." << endl;
        }
        
        // Remove from allTransactions
        for (size_t i = 0; i < allTransactions.size(); i++) {
            if (allTransactions[i] == Transaction_History[choice-1]) {
                allTransactions.erase(allTransactions.begin() + i);
                break;
            }
        }
        
        // Remove from history
        Transaction_History.erase(Transaction_History.begin() + (choice-1));
        cout << "Order cancelled successfully." << endl;
    }
}

void Customer::Select_Shop() {
    if (allShops.empty()) {
        cout << "No shops available!" << endl;
        return;
    }
    
    cout << "\n=== Select Shop ===" << endl;
    for (size_t i = 0; i < allShops.size(); i++) {
        cout << i+1 << ". " << allShops[i]->Shop_Name 
             << " (" << allShops[i]->Timings << ")" << endl;
    }
    
    int choice;
    cout << "Select shop (0 to cancel): ";
    cin >> choice;
    
    if (choice > 0 && choice <= (int)allShops.size()) {
        cout << "Selected shop: " << allShops[choice-1]->Shop_Name << endl;
    }
}

void Customer::showMenu() {
    while (true) {
        clearScreen();
        cout << "\n=== CUSTOMER DASHBOARD ===" << endl;
        cout << "Welcome, " << Name << "!" << endl;
        cout << "Wallet: Rs." << Wallet_Balance << " | Credit: Rs." << Credit_Balance << endl;
        cout << "\n1. View Available Shops" << endl;
        cout << "2. View All Items" << endl;
        cout << "3. Add Item to Cart" << endl;
        cout << "4. View Cart (" << Cart.size() << " items)" << endl;
        cout << "5. Remove Item from Cart" << endl;
        cout << "6. Place Order" << endl;
        cout << "7. View Order History" << endl;
        cout << "8. Add Money to Wallet" << endl;
        cout << "9. View Profile" << endl;
        cout << "10. Logout" << endl;
        cout << "Choice: ";
        
        int choice;
        cin >> choice;
        
        switch(choice) {
            case 1:
                clearScreen();
                cout << "\n=== Available Shops ===" << endl;
                for (size_t i = 0; i < allShops.size(); i++) {
                    cout << i+1 << ". " << allShops[i]->Shop_Name 
                         << " (" << allShops[i]->Timings << ")" << endl;
                    cout << "   " << allShops[i]->Description << endl;
                }
                pressEnterToContinue();
                break;
            case 2:
                clearScreen();
                cout << "\n=== All Items ===" << endl;
                for (size_t i = 0; i < allItems.size(); i++) {
                    allItems[i]->display();
                }
                pressEnterToContinue();
                break;
            case 3:
                clearScreen();
                Add_Cart();
                pressEnterToContinue();
                break;
            case 4:
                clearScreen();
                displayCart();
                pressEnterToContinue();
                break;
            case 5:
                clearScreen();
                Remove_Cart();
                pressEnterToContinue();
                break;
            case 6:
                clearScreen();
                placeOrder();
                pressEnterToContinue();
                break;
            case 7:
                clearScreen();
                viewOrderHistory();
                pressEnterToContinue();
                break;
            case 8:
                clearScreen();
                Add_moneyToWallet();
                pressEnterToContinue();
                break;
            case 9:
                clearScreen();
                display();
                pressEnterToContinue();
                break;
            case 10:
                cout << "Logging out..." << endl;
                return;
            default:
                cout << "Invalid choice!" << endl;
                pressEnterToContinue();
        }
    }
}

// Shopkeeper methods
Shopkeeper::Shopkeeper(string name, string email, string password, string contact, string accNo)
    : User(name, email, password, contact, "Shopkeeper"),
      Account_No(accNo), Pending_Amount(0), myShop(NULL) {}

void Shopkeeper::Shop_Registration() {
    if (myShop != NULL) {
        cout << "You already have a registered shop!" << endl;
        return;
    }
    
    cin.ignore();
    string shopName, timings, description;
    
    cout << "\n=== Shop Registration ===" << endl;
    cout << "Shop Name: ";
    getline(cin, shopName);
    cout << "Shop Timings (e.g., 9 AM - 10 PM): ";
    getline(cin, timings);
    cout << "Shop Description: ";
    getline(cin, description);
    
    myShop = new Shop(shopName, timings, description, this);
    allShops.push_back(myShop);
    cout << "Shop '" << shopName << "' registered successfully!" << endl;
}

void Shopkeeper::Update_Records() {
    cout << "\n=== Transaction Records ===" << endl;
    if (Transaction_History.empty()) {
        cout << "No transactions yet." << endl;
    } else {
        for (size_t i = 0; i < Transaction_History.size(); i++) {
            Transaction_History[i]->Get_details();
            cout << endl;
        }
    }
}

void Shopkeeper::add_new_item() {
    if (myShop == NULL) {
        cout << "Please register a shop first!" << endl;
        return;
    }
    
    cin.ignore();
    string name, description;
    double price;
    int quantity;
    
    cout << "\n=== Add New Item ===" << endl;
    cout << "Item Name: ";
    getline(cin, name);
    cout << "Description: ";
    getline(cin, description);
    cout << "Price: Rs.";
    cin >> price;
    cout << "Quantity: ";
    cin >> quantity;
    cin.ignore();
    
    Item* newItem = new Item(name, price, quantity, description, myShop);
    
    // Add keywords
    cout << "Add keywords (separate by space, press Enter when done): ";
    string keywords;
    getline(cin, keywords);
    
    stringstream ss(keywords);
    string keyword;
    while (ss >> keyword) {
        newItem->addKeyword(keyword);
    }
    
    myShop->addItem(newItem);
    cout << "Item added successfully!" << endl;
}

void Shopkeeper::add_new_holiday() {
    cin.ignore();
    string holiday;
    cout << "Enter holiday name/date: ";
    getline(cin, holiday);
    Holiday_List.push_back(holiday);
    cout << "Holiday added: " << holiday << endl;
}

void Shopkeeper::displayHolidays() const {
    if (Holiday_List.empty()) {
        cout << "No holidays scheduled." << endl;
        return;
    }
    cout << "\n--- Holiday List ---" << endl;
    for (size_t i = 0; i < Holiday_List.size(); i++) {
        cout << " - " << Holiday_List[i] << endl;
    }
}

void Shopkeeper::viewShopItems() {
    if (myShop == NULL) {
        cout << "No shop registered!" << endl;
        return;
    }
    myShop->displayItems();
}

void Shopkeeper::showMenu() {
    while (true) {
        clearScreen();
        cout << "\n=== SHOPKEEPER DASHBOARD ===" << endl;
        cout << "Welcome, " << Name << "!" << endl;
        if (myShop) {
            cout << "Shop: " << myShop->Shop_Name << endl;
        }
        cout << "\n1. Register Shop" << endl;
        cout << "2. View My Shop" << endl;
        cout << "3. Add New Item" << endl;
        cout << "4. View Shop Items" << endl;
        cout << "5. View Transaction Records" << endl;
        cout << "6. Add Holiday" << endl;
        cout << "7. View Holidays" << endl;
        cout << "8. View Profile" << endl;
        cout << "9. Logout" << endl;
        cout << "Choice: ";
        
        int choice;
        cin >> choice;
        
        switch(choice) {
            case 1:
                clearScreen();
                Shop_Registration();
                pressEnterToContinue();
                break;
            case 2:
                clearScreen();
                if (myShop) {
                    myShop->displayItems();
                } else {
                    cout << "No shop registered yet!" << endl;
                }
                pressEnterToContinue();
                break;
            case 3:
                clearScreen();
                add_new_item();
                pressEnterToContinue();
                break;
            case 4:
                clearScreen();
                viewShopItems();
                pressEnterToContinue();
                break;
            case 5:
                clearScreen();
                Update_Records();
                pressEnterToContinue();
                break;
            case 6:
                clearScreen();
                add_new_holiday();
                pressEnterToContinue();
                break;
            case 7:
                clearScreen();
                displayHolidays();
                pressEnterToContinue();
                break;
            case 8:
                clearScreen();
                display();
                pressEnterToContinue();
                break;
            case 9:
                cout << "Logging out..." << endl;
                return;
            default:
                cout << "Invalid choice!" << endl;
                pressEnterToContinue();
        }
    }
}

// DeliveryBoy methods
DeliveryBoy::DeliveryBoy(string name, string email, string password, string contact)
    : User(name, email, password, contact, "DeliveryBoy"),
      Availability(true) {}

void DeliveryBoy::addTransaction(Transaction* t) {
    Transaction_History.push_back(t);
}

void DeliveryBoy::viewAssignedDeliveries() {
    cout << "\n=== Assigned Deliveries ===" << endl;
    if (Transaction_History.empty()) {
        cout << "No deliveries assigned." << endl;
    } else {
        for (size_t i = 0; i < Transaction_History.size(); i++) {
            cout << i+1 << ". ";
            Transaction_History[i]->Get_details();
            cout << endl;
        }
    }
}

void DeliveryBoy::toggleAvailability() {
    Availability = !Availability;
    cout << "Availability set to: " << (Availability ? "Available" : "Busy") << endl;
}

void DeliveryBoy::showMenu() {
    while (true) {
        clearScreen();
        cout << "\n=== DELIVERY BOY DASHBOARD ===" << endl;
        cout << "Welcome, " << Name << "!" << endl;
        cout << "Status: " << (Availability ? "Available" : "Busy") << endl;
        cout << "Assigned Deliveries: " << Transaction_History.size() << endl;
        cout << "\n1. View Assigned Deliveries" << endl;
        cout << "2. Toggle Availability" << endl;
        cout << "3. View Profile" << endl;
        cout << "4. Logout" << endl;
        cout << "Choice: ";
        
        int choice;
        cin >> choice;
        
        switch(choice) {
            case 1:
                clearScreen();
                viewAssignedDeliveries();
                pressEnterToContinue();
                break;
            case 2:
                clearScreen();
                toggleAvailability();
                pressEnterToContinue();
                break;
            case 3:
                clearScreen();
                display();
                pressEnterToContinue();
                break;
            case 4:
                cout << "Logging out..." << endl;
                return;
            default:
                cout << "Invalid choice!" << endl;
                pressEnterToContinue();
        }
    }
}

// Transaction methods
Transaction::Transaction(Customer* cust, Shop* sh, string payType)
    : customer(cust), shop(sh), Payment_Type(payType), 
      Delivery_Boy(NULL), Payment_status("Pending"),
      totalAmount(0) {
    transactionCount++;
    Transaction_ID = "TXN" + intToString(transactionCount);
    date = Date::getToday();
}

bool Transaction::is_possible() const {
    if (!customer) {
        cout << "No customer assigned." << endl;
        return false;
    }
    if (customer->isDefaulter()) {
        cout << "Customer is a defaulter." << endl;
        return false;
    }
    if (items.empty()) {
        cout << "No items in transaction." << endl;
        return false;
    }
    if (delivery_place.empty()) {
        cout << "Delivery address not set." << endl;
        return false;
    }
    return true;
}

void Transaction::Get_details() const {
    cout << "\n=== Transaction Details ===" << endl;
    cout << "Transaction ID: " << Transaction_ID << endl;
    cout << "Date: " << date.toString() << endl;
    cout << "Customer: " << (customer ? customer->getName() : "None") << endl;
    cout << "Delivery Address: " << delivery_place << endl;
    cout << "Delivery Slot: " << Delivery_slot.toString() << endl;
    cout << "Payment Type: " << Payment_Type << endl;
    cout << "Payment Status: " << Payment_status << endl;
    cout << "Delivery Boy: " << (Delivery_Boy ? Delivery_Boy->getName() : "None") << endl;
    
    cout << "\nItems (" << items.size() << "):" << endl;
    for (size_t i = 0; i < items.size(); i++) {
        cout << " - " << items[i]->name << " | Rs." << items[i]->Price << endl;
    }
    cout << "Total Amount: Rs." << totalAmount << endl;
}

void Transaction::CheckOut() {
    if (!is_possible()) {
        cout << "Checkout not possible." << endl;
        return;
    }

    // Calculate total
    totalAmount = 0;
    for (size_t i = 0; i < items.size(); i++) {
        totalAmount += items[i]->Price;
    }

    // Process payment
    if (Payment_Type == "Wallet") {
        if (customer->getWalletBalance() >= totalAmount) {
            customer->setWalletBalance(customer->getWalletBalance() - totalAmount);
            Payment_status = "Paid";
            cout << "Payment successful from wallet." << endl;
        } else {
            Payment_status = "Failed";
            cout << "Insufficient wallet balance." << endl;
            return;
        }
    } else if (Payment_Type == "Credit") {
        if (customer->getCreditBalance() >= totalAmount) {
            customer->setCreditBalance(customer->getCreditBalance() - totalAmount);
            Payment_status = "Credit";
            cout << "Charged to credit balance." << endl;
        } else {
            Payment_status = "Failed";
            cout << "Insufficient credit balance." << endl;
            return;
        }
    } else { // Cash
        Payment_status = "Cash_Pending";
        cout << "Cash payment to be collected on delivery." << endl;
    }
}

void Transaction::addItem(const Item& item) { 
    // Find the actual item pointer
    for (size_t i = 0; i < allItems.size(); i++) {
        if (allItems[i]->ID == item.ID) {
            items.push_back(allItems[i]);
            totalAmount += allItems[i]->Price;
            break;
        }
    }
}

// Moderator methods
Moderator::Moderator(string name, string email, string password, string contact)
    : User(name, email, password, contact, "Moderator"),
      Available_balance(10000) {
    systemAdmin = this;
}

DeliveryBoy* Moderator::Find_delivery_boy() {
    for (size_t i = 0; i < Delivery_Boys.size(); i++) {
        if (Delivery_Boys[i]->isAvailable()) {
            return Delivery_Boys[i];
        }
    }
    return NULL;
}

void Moderator::Assign_delivery_boy(Transaction* transaction) {
    DeliveryBoy* db = Find_delivery_boy();
    if (db) {
        transaction->setDeliveryBoy(db);
        db->setAvailability(false);
        db->addTransaction(transaction);
        cout << "Delivery boy " << db->getName() << " assigned to transaction." << endl;
    } else {
        cout << "No delivery boy available." << endl;
    }
}

void Moderator::Collect_credit() {
    if (Customers.empty()) {
        cout << "No customers in system." << endl;
        return;
    }
    
    cout << "\n=== Collect Credit ===" << endl;
    for (size_t i = 0; i < Customers.size(); i++) {
        cout << i+1 << ". " << Customers[i]->getName() 
             << " | Credit Balance: Rs." << Customers[i]->getCreditBalance() << endl;
    }
    
    int choice;
    cout << "Select customer (0 to cancel): ";
    cin >> choice;
    
    if (choice > 0 && choice <= (int)Customers.size()) {
        double amount;
        cout << "Enter amount to collect: Rs.";
        cin >> amount;
        
        if (amount > 0 && amount <= Customers[choice-1]->getCreditBalance()) {
            Customers[choice-1]->setCreditBalance(Customers[choice-1]->getCreditBalance() - amount);
            Available_balance += amount;
            cout << "Collected Rs." << amount << " from " << Customers[choice-1]->getName() << endl;
        } else {
            cout << "Invalid amount!" << endl;
        }
    }
}

void Moderator::pay_shopkeeper() {
    if (Shopkeepers.empty()) {
        cout << "No shopkeepers in system." << endl;
        return;
    }
    
    cout << "\n=== Pay Shopkeeper ===" << endl;
    for (size_t i = 0; i < Shopkeepers.size(); i++) {
        Shopkeeper* sk = dynamic_cast<Shopkeeper*>(Shopkeepers[i]);
        if (sk) {
            cout << i+1 << ". " << sk->getName() 
                 << " | Pending Amount: Rs." << sk->getPendingAmount() << endl;
        }
    }
    
    int choice;
    cout << "Select shopkeeper (0 to cancel): ";
    cin >> choice;
    
    if (choice > 0 && choice <= (int)Shopkeepers.size()) {
        double amount;
        cout << "Enter amount to pay: Rs.";
        cin >> amount;
        
        Shopkeeper* sk = dynamic_cast<Shopkeeper*>(Shopkeepers[choice-1]);
        if (sk && amount > 0 && amount <= sk->getPendingAmount() && amount <= Available_balance) {
            sk->setPendingAmount(sk->getPendingAmount() - amount);
            Available_balance -= amount;
            cout << "Paid Rs." << amount << " to " << sk->getName() << endl;
        } else {
            cout << "Invalid amount!" << endl;
        }
    }
}

void Moderator::mark_defaulters() {
    cout << "\n=== Mark Defaulters ===" << endl;
    int count = 0;
    for (size_t i = 0; i < Customers.size(); i++) {
        if (Customers[i]->getCreditBalance() < 0) {
            Customers[i]->setDefaulter(true);
            cout << Customers[i]->getName() << " marked as defaulter." << endl;
            count++;
        }
    }
    if (count == 0) {
        cout << "No defaulters found." << endl;
    }
}

void Moderator::add_new_holiday() {
    cin.ignore();
    string holiday;
    cout << "Enter system holiday: ";
    getline(cin, holiday);
    holiday_list.push_back(holiday);
    cout << "System holiday added: " << holiday << endl;
}

void Moderator::Generate_spreadsheet() {
    ofstream file("system_report.csv");
    if (file.is_open()) {
        file << "User Type,Name,Email,Transactions/Balance\n";
        
        // Customers
        for (size_t i = 0; i < Customers.size(); i++) {
            Customer* cust = dynamic_cast<Customer*>(Customers[i]);
            if (cust) {
                file << "Customer," << cust->getName() << "," << cust->getEmail()
                     << ",Wallet: Rs." << cust->getWalletBalance() 
                     << ",Credit: Rs." << cust->getCreditBalance() << "\n";
            }
        }
        
        // Shopkeepers
        for (size_t i = 0; i < Shopkeepers.size(); i++) {
            Shopkeeper* sk = dynamic_cast<Shopkeeper*>(Shopkeepers[i]);
            if (sk) {
                file << "Shopkeeper," << sk->getName() << "," << sk->getEmail()
                     << ",Pending: Rs." << sk->getPendingAmount() << "\n";
            }
        }
        
        file.close();
        cout << "Spreadsheet generated: system_report.csv" << endl;
    }
}

void Moderator::viewSystemStats() {
    cout << "\n=== SYSTEM STATISTICS ===" << endl;
    cout << "Total Users: " << allUsers.size() << endl;
    cout << "Total Shops: " << allShops.size() << endl;
    cout << "Total Items: " << allItems.size() << endl;
    cout << "Total Transactions: " << allTransactions.size() << endl;
    cout << "Moderator Balance: Rs." << Available_balance << endl;
    
    cout << "\nUser Breakdown:" << endl;
    int customers = 0, shopkeepers = 0, deliveryboys = 0;
    for (size_t i = 0; i < allUsers.size(); i++) {
        if (allUsers[i]->getUserType() == "Customer") customers++;
        else if (allUsers[i]->getUserType() == "Shopkeeper") shopkeepers++;
        else if (allUsers[i]->getUserType() == "DeliveryBoy") deliveryboys++;
    }
    cout << "Customers: " << customers << endl;
    cout << "Shopkeepers: " << shopkeepers << endl;
    cout << "Delivery Boys: " << deliveryboys << endl;
}

void Moderator::showMenu() {
    while (true) {
        clearScreen();
        cout << "\n=== MODERATOR ADMIN PANEL ===" << endl;
        cout << "Welcome, " << Name << "!" << endl;
        cout << "System Balance: Rs." << Available_balance << endl;
        cout << "\n1. View System Statistics" << endl;
        cout << "2. Generate System Report" << endl;
        cout << "3. Collect Credit from Customers" << endl;
        cout << "4. Pay Shopkeepers" << endl;
        cout << "5. Mark Defaulters" << endl;
        cout << "6. Add System Holiday" << endl;
        cout << "7. View All Users" << endl;
        cout << "8. View All Transactions" << endl;
        cout << "9. View Profile" << endl;
        cout << "10. Logout" << endl;
        cout << "Choice: ";
        
        int choice;
        cin >> choice;
        
        switch(choice) {
            case 1:
                clearScreen();
                viewSystemStats();
                pressEnterToContinue();
                break;
            case 2:
                clearScreen();
                Generate_spreadsheet();
                pressEnterToContinue();
                break;
            case 3:
                clearScreen();
                Collect_credit();
                pressEnterToContinue();
                break;
            case 4:
                clearScreen();
                pay_shopkeeper();
                pressEnterToContinue();
                break;
            case 5:
                clearScreen();
                mark_defaulters();
                pressEnterToContinue();
                break;
            case 6:
                clearScreen();
                add_new_holiday();
                pressEnterToContinue();
                break;
            case 7:
                clearScreen();
                cout << "\n=== ALL USERS ===" << endl;
                for (size_t i = 0; i < allUsers.size(); i++) {
                    cout << i+1 << ". ";
                    allUsers[i]->display();
                    cout << endl;
                }
                pressEnterToContinue();
                break;
            case 8:
                clearScreen();
                cout << "\n=== ALL TRANSACTIONS ===" << endl;
                for (size_t i = 0; i < allTransactions.size(); i++) {
                    allTransactions[i]->Get_details();
                    cout << endl;
                }
                pressEnterToContinue();
                break;
            case 9:
                clearScreen();
                display();
                pressEnterToContinue();
                break;
            case 10:
                cout << "Logging out..." << endl;
                return;
            default:
                cout << "Invalid choice!" << endl;
                pressEnterToContinue();
        }
    }
}

// =============================
// SYSTEM INITIALIZATION
// =============================
void initializeSystem() {
    // Create system admin
    systemAdmin = new Moderator("System Admin", "admin@system.com", "admin123", "0000-111111");
    allUsers.push_back(systemAdmin);
    
    // Create some sample data
    Shopkeeper* sk1 = new Shopkeeper("Ali", "ali@shop.com", "pass123", "0300-1234567", "ACC001");
    Shopkeeper* sk2 = new Shopkeeper("Fatima", "fatima@shop.com", "pass456", "0311-7654321", "ACC002");
    
    Customer* cust1 = new Customer("Zainab", "zainab@cust.com", "cust123", "0320-1112222");
    Customer* cust2 = new Customer("Ahmed", "ahmed@cust.com", "cust456", "0333-3334444");
    
    DeliveryBoy* db1 = new DeliveryBoy("Hamza", "hamza@delivery.com", "db111", "0344-5556666");
    DeliveryBoy* db2 = new DeliveryBoy("Sajid", "sajid@delivery.com", "db222", "0355-7778888");
    
    allUsers.push_back(sk1);
    allUsers.push_back(sk2);
    allUsers.push_back(cust1);
    allUsers.push_back(cust2);
    allUsers.push_back(db1);
    allUsers.push_back(db2);
    
    // Create shops directly (without prompting for input)
    Shop* shop1 = new Shop("Ali's Grocery Store", "9 AM - 10 PM", "Fresh groceries and daily essentials", sk1);
    Shop* shop2 = new Shop("Fatima's Supermart", "8 AM - 9 PM", "Everything you need under one roof", sk2);
    allShops.push_back(shop1);
    allShops.push_back(shop2);
    sk1->setShop(shop1);
    sk2->setShop(shop2);
    
    // Add sample items
    Item* item1 = new Item("Milk", 120, 50, "Fresh full cream milk", shop1);
    item1->addKeyword("dairy");
    item1->addKeyword("milk");
    item1->addKeyword("fresh");
    
    Item* item2 = new Item("Bread", 80, 100, "Whole wheat bread", shop1);
    item2->addKeyword("bakery");
    item2->addKeyword("bread");
    item2->addKeyword("wheat");
    
    Item* item3 = new Item("Eggs", 200, 30, "Farm fresh eggs (dozen)", shop2);
    item3->addKeyword("dairy");
    item3->addKeyword("eggs");
    item3->addKeyword("farm");
    
    Item* item4 = new Item("Rice", 450, 20, "Basmati rice 5kg", shop2);
    item4->addKeyword("grocery");
    item4->addKeyword("rice");
    item4->addKeyword("basmati");
    
    // Add to moderator lists
    systemAdmin->addCustomer(cust1);
    systemAdmin->addCustomer(cust2);
    systemAdmin->addShopkeeper(sk1);
    systemAdmin->addShopkeeper(sk2);
    systemAdmin->addDeliveryBoy(db1);
    systemAdmin->addDeliveryBoy(db2);
}

// =============================
// MAIN PROGRAM - USER INTERFACE
// =============================
void mainMenu() {
    while (true) {
        clearScreen();
        cout << "=========================================" << endl;
        cout << "      ONLINE DELIVERY SYSTEM v1.0" << endl;
        cout << "=========================================" << endl;
        cout << "\n1. Login" << endl;
        cout << "2. Register" << endl;
        cout << "3. Exit" << endl;
        cout << "\nChoice: ";
        
        int choice;
        cin >> choice;
        
        switch(choice) {
            case 1:
                clearScreen();
                currentUser = User::login();
                if (currentUser) {
                    currentUser->showMenu();
                    currentUser = NULL;
                } else {
                    pressEnterToContinue();
                }
                break;
            case 2:
                clearScreen();
                User::registerUser();
                pressEnterToContinue();
                break;
            case 3:
                cout << "\nThank you for using Online Delivery System!" << endl;
                cout << "Total Users: " << User::getUserCount() << endl;
                cout << "Total Items: " << Item::getItemCount() << endl;
                return;
            default:
                cout << "Invalid choice!" << endl;
                pressEnterToContinue();
        }
    }
}

int main() {
    // Initialize system with sample data
    initializeSystem();
    
    // Start the main menu
    mainMenu();
    
    // Cleanup (in a real system, you'd want proper cleanup)
    return 0;
}