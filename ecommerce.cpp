#include<bits/stdc++.h>
using namespace std;

struct user{
	string name;
	string gender;
	int age;
	vector<string>transactions;
};

struct product{
	string sellerName;
	string productName;
	double price;
	double rating;
	int quantity;
};
//Database
vector<user>users;
vector<product>products;
unordered_map<string, int>commandsMap;

void createCommandMap(){
	commandsMap["add_user"] = 1;
	commandsMap["add_product"] = 2;
	commandsMap["select_product"] = 3;
	commandsMap["list_transaction_summary"] = 4;
	commandsMap["cancel_purchase"] = 5;
}
vector<string> split (const string& s, char delim) {
    vector<string> result;
    stringstream ss (s);
    string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }
    return result;
}
vector<product*> getProduct(string productName){
	vector<product*>result;
	for(product& p : products){
		if(p.productName == productName && p.quantity > 0){
			result.push_back(&p);
		}
	}
	return result;
}
product* getProductBySeller(string productName, string sellerName){
    for(product& p : products){
		if(p.productName == productName && p.sellerName == sellerName){
			return &p;
		}
	}
	return nullptr;
}
user* getUser(string sellerName){
	for(user& u: users){
		if(u.name == sellerName)
			return &u;
	}
	return nullptr;
}
void addUser(string name, string gender, int age, vector<string>transactions){
	user u = {name, gender, age, transactions};
	users.push_back(u);
}
void addProduct(string sellerName, string productName, double price, double rating, int quantity){
	user* seller = getUser(sellerName);
	if(seller == nullptr){
		cout<<"User not found on the platform!"<<endl;
		return;
	}else{
		string trans = "SELL :" + productName;
		seller->transactions.push_back(trans);
	}
	product p = {sellerName, productName, price, rating, quantity};
	products.push_back(p);
}

void buyProduct(string buyerName, string productName, string criteria){
	user* buyer = getUser(buyerName);
	if(buyer == nullptr){
		cout<<"Buyer not found on the platform!"<<endl;
		return;
	}
	vector<product*>interestedProduct = getProduct(productName);
	if(interestedProduct.size() == 0){
		cout<<"No such product found!"<<endl;
		return;
	}
	if(criteria == "best_rating"){
		sort(interestedProduct.begin(), interestedProduct.end(), [](product* a, product* b){
				if(a->rating != b->rating)
					return a->rating > b->rating;
				return a->price < b->price;
		});

	}else if(criteria == "best_price"){
		sort(interestedProduct.begin(), interestedProduct.end(), [](product* a, product* b){
				if(a->price != b->price)
					return a->price < b->price;
				return a->rating > b->rating;
		});
	}
	auto topProduct = interestedProduct[0];
	//decrease the quantity by one
	topProduct->quantity -= 1;
	cout<<buyerName<<" bought a :"<<topProduct->productName<<" from :"<<topProduct->sellerName<<" at :"<<topProduct->price<<endl;
	//update buyer transaction
	string trans = "BUY :" + topProduct->productName;
	buyer->transactions.push_back(trans);
}
void listTransactionSummary(){
	for(user u : users){
        cout<<"Transaction summary for :"<<u.name<<endl;
        if(u.transactions.size() == 0){
            cout<<"No transactions for this use yet"<<endl;
            continue;
        }
		for(string trans : u.transactions){
			cout<<trans<<" ";
		}
		cout<<endl;
	}
}
void cancelPurchase(string buyerName, string sellerName, string productName){
    user* buyer = getUser(buyerName);
    user* seller = getUser(sellerName);
    if(buyer == nullptr || seller == nullptr){
        cout<<"User does not exist"<<endl;
        return;
    }
    product* cancelProduct = getProductBySeller(productName, sellerName);
    if(cancelProduct == nullptr){
        cout<<"The product does not exist"<<endl;
        return;
    }else{
        cancelProduct->quantity += 1;
        string cancelTrans = "Cancelled :" + cancelProduct->productName;
        buyer->transactions.push_back(cancelTrans);
        cout<<"Purchase cancelled!"<<endl;
    }
}
int main(){
	createCommandMap();
	string in = "";
	while(in != "quit"){
        cout<<"Enter a command :"<<endl;
		getline(cin, in);
		vector<string>commands = split(in, ' ');
		string command = commands[0];
		switch(commandsMap[command]){
			case 1:
			{
				string name = commands[1];
				string gender = commands[2];
				int age = stoi(commands[3]);
				vector<string>transactions;
				addUser(name, gender, age, transactions);
				cout<<"User added!"<<endl;
                break;
			}
			case 2:
			{
				string sellerName = commands[1];
				string productName = commands[2];
				double price = stod(commands[3]);
				double rating = stod(commands[4]);
				int quantity = stoi(commands[5]);
				addProduct(sellerName, productName, price, rating, quantity);
				cout<<"Product added!"<<endl;
				break;
			}
			case 3:
			{
				string buyerName = commands[1];
				string productName = commands[2];
				string criteria = commands[3];
				buyProduct(buyerName, productName, criteria);
				break;
			}
			case 4:
			{
				listTransactionSummary();
				break;
			}
			case 5:
            {
                string buyerName = commands[1];
                string sellerName = commands[2];
                string productName = commands[3];
                cancelPurchase(buyerName, sellerName, productName);
                break;
            }
			default:
			{
				cout<<"Wrong command!"<<endl;
			}
		}
	}
}
