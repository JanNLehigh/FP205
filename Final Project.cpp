#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <iomanip>
#include <cmath>

using namespace std;

struct Node{ //struct for the tree
    string value; //value store in the node
    Node *left, *right; //branches to the next leaf
    Node(string val){
        value = val;
        left = nullptr;
        right = nullptr;
    }
};

int getPrecedence(char op){//this function will be used in the postfix, sets precedence based on the the operator
    if(op == '^'){//highest precedence
        return 3;
    }else if(op == '*' || op == '/'){ 
        return 2;
    }else if(op == '+' || op == '-'){ 
        return 1;
    }else{   
        return 0; // lowest precedence for digits
    }
}

// To make a shunting yard (from infix to postfix)
vector<string> infixToPostfix(string infix){
    vector<string> output; //vector to store the postfix
    stack<char> poststack; //stack to seperate the infix in order to sort in postfix, used to make transfer and sorting easier

    for (int i = 0; i < infix.length(); i++){
        if(infix[i]== ' '){ //skip the spaces
            continue;
        }else if(isdigit(infix[i])){        // if it's a number, collect it in the output
            string num = "";
            while (i < infix.length() && isdigit(infix[i])) {
                num += infix[i++];
            }
            output.push_back(num);
            i--; // Adjust for outer loop increment
        }else if (infix[i] == '('){ //check for parentheses
            poststack.push('(');
        }else if (infix[i] == ')'){
            while(!poststack.empty() && poststack.top() != '('){ //if the postfix stack is not empty and the top of the stack is not the first parenthesis..
                output.push_back(string(1, poststack.top())); //..add the number to the stack
                poststack.pop();
            }
            if (!poststack.empty()){// pop the ('
                poststack.pop();
            } 
        }else{
            while (!poststack.empty() && poststack.top() != '(' && getPrecedence(poststack.top()) >= getPrecedence(infix[i])){//sort the remaining
                output.push_back(string(1, poststack.top()));
               poststack.pop();
            }
            poststack.push(infix[i]);
        }
    }

    while (!poststack.empty()) {
        output.push_back(string(1, poststack.top()));
        poststack.pop();
    }
    return output;
}

//binary expression tree from Postfix
Node* buildTree(const vector<string>& postfix) {
    stack<Node*> s;
    for (const string& token : postfix) {
        // If the first char is a number, make a node
        if (isdigit(token[0])) {
            s.push(new Node(token));
        }else{ //if char is a operator
            Node* node = new Node(token);
            // right child is popped first
            node->right = s.top(); 
            s.pop();
            node->left = s.top(); 
            s.pop();
            s.push(node);
        }
    }
    return s.top();
}
void printPostfix(Node* root) { //function to print the node values
    if(!root){ //If there's are no leaves, just print the digit
        return;
    }
    printPostfix(root->left);
    printPostfix(root->right);
    cout << root->value << " ";
}
//The following functions are used to created the tree
int getHeight(Node* root){
    if(!root){
        return 0; //if it's only the root, there's no height, just display the root 
    }else{
        return 1 + max(getHeight(root->left), getHeight(root->right)); //Find the max amount of branches and add one for the root
    }
}

void printLevel(Node* root, int level, int indent, int spacing){ //helps with the visual
    if (level == 1){ //first level
        if(root){//if a root exist, displays it in the middle
            cout << setw(indent) << root->value << setw(spacing) << " ";
        }else{
            cout << setw(indent) << " " << setw(spacing) << " ";
        }
        return;
    }

    if(root){ //if there are leaves of the root, print them
        printLevel(root->left, level - 1, indent, spacing);
        printLevel(root->right, level - 1, indent, spacing);
    } else {
        //If there is no leaves, print nothing
        printLevel(nullptr, level - 1, indent, spacing);
        printLevel(nullptr, level - 1, indent, spacing);
    }
}


void printTree(Node* root){//display Function
    int height = getHeight(root);
    for (int i = 1; i <= height; i++) {
        //to calculates spacing based on tree height
        int indent = pow(2, height - i) * 2;
        int spacing = pow(2, height - i + 1);
        
        // offset the very first indent of each line
        cout << string(indent, ' ');
        printLevel(root, i, 0, spacing);
        cout << endl; 
    }
}


int evaluate(Node* root){//Evulation function
    if (!root){ //no root 
        return 0;
    }
    // If root has no leaves, convert root value from an string to an integer
    if (!root->left && !root->right){
        return stoi(root->value);
    }

    int leftVal = evaluate(root->left); //using the function to evulate the left leaf
    int rightVal = evaluate(root->right); //using the function to evulate the right leaf

    if (root->value == "+"){ //add operator
        return leftVal + rightVal;
    }
    if (root->value == "-"){//sub operator
        return leftVal - rightVal;
    }
    if (root->value == "*"){//multiply operator
        return leftVal * rightVal;
    }
    if (root->value == "/"){ //divider operator
        if(rightVal != 0){
            return leftVal / rightVal;
        }else{
            return 0;
        }
    }
    if (root->value == "^"){ //expontential operator
        return pow(leftVal, rightVal);
    }
    return 0;
}

int main() {
    string infix;
    cout << "Input a infix expression" << endl;
    getline(cin, infix); //better than regualr cin, this allows spaces
    cout << "Input Infix: " << infix << endl;

    vector<string> postfix = infixToPostfix(infix);
    Node* treeRoot = buildTree(postfix);
    cout << "Postfix: "; // makes and displays the postfix
    printPostfix(treeRoot);
    cout << endl;

    cout << "Tree: ";//displays the tree
    printTree(treeRoot);

    cout << "Evaluation Result: " << evaluate(treeRoot) << endl; //displays the evulation

    return 0;
}