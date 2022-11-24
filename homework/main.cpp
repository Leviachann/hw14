#include <iostream>
using namespace std;
class Exception {};
class DatabaseException : public Exception {
public:
  string message;
  DatabaseException(string message) { this->message = message; }

  string Message() const { return message; }
};
class InvalidArgumentException : public Exception {
public:
  string message;
  InvalidArgumentException(string message) { this->message = message; }
  string Message() const { return message; }
};
static int main_id;
class User {
public:
  int id = main_id++;
  string username;
  string password;
  string name;
  string surname;
  User(string username, string password, string name, string surname) {
    this->username = username;
    this->password = password;
    this->name = name;
    this->surname = surname;
  }
  bool operator==(const User &us) {
    return name == us.name && password == us.password;
  }
  void show() const {
    cout << "Username: " << username << endl;
    cout << "Name: " << name << endl;
    cout << "Surname: " << surname << endl;
  };
};

class Database {
public:
  User **users;
  int user_count = 0;
  void addUser(const User &user) {
    if (user_count == 0) {
      users = new User *[user_count + 1];
      users[0] =
          new User(user.username, user.password, user.name, user.surname);
    } else {
      users = (User **)realloc(users, sizeof(User *) * (user_count + 1));
      users[user_count] =
          new User(user.username, user.password, user.name, user.surname);
    }
  }
  User &getUserByUsername(string username) {
    for (int i = 0; i < user_count; i++) {
      if (users[i]->username == username) {
        return *users[i];
      }
    }
  }
  void updateUser(User &olduser, const User &newuser) {
    if (olduser.username == newuser.username) {
      olduser.name = newuser.name;
      olduser.surname = newuser.surname;
    }
  }
  void deleteUserById(const int &id) {
    for (int i = 0; i < user_count; i++) {
      if (users[i]->id == id) {
        delete users[i];
        users[i] = NULL;
        user_count--;
        break;
      }
    }
  }
  void hashUserDataById(const int &id) {
    const std::hash<std::string> hasher;
    for (int i = 0; i < user_count; i++) {
      if (users[i]->id == id) {
        const auto hashname = hasher(users[i]->name);
        const auto hashusername = hasher(users[i]->username);
        const auto hashsurname = hasher(users[i]->surname);
      }
    }
  }
};
class Registration {
  Database _database;

public:
  Registration(const Database &database) { _database = database; }
  void signIn(string username, string password) {
    for (int i = 0; i < _database.user_count; i++) {
      if (_database.users[i]->username == username) {
        if (_database.users[i]->password == password)
          cout << "User " << username << " is signed in." << endl;
        else
          throw DatabaseException("Invalid password.");
      }
    }
    throw DatabaseException("User not found.");
  };
  void signUp(string username, string password, string name, string surname) {
    for (int i = 0; i < _database.user_count; i++) {
      if (_database.users[i]->username == username) {
        throw DatabaseException("Username is already taken");
      }

      if (username.length() == 0) {
        throw InvalidArgumentException("Username is empty");
      }
      if (password.length() == 0) {
        throw InvalidArgumentException("Password is empty");
      }
      if (name.length() == 0) {
        throw InvalidArgumentException("Name is empty");
      }
      if (surname.length() == 0) {
        throw InvalidArgumentException("Surname is empty");
      }
      if (username.length() < 6) {
        throw InvalidArgumentException("Username is too short");
      }
      if (password.length() < 6) {
        throw InvalidArgumentException("Password is too short");
      }
      if (name.length() < 4) {
        throw InvalidArgumentException("Name is too short");
      }
      if (surname.length() < 4) {
        throw InvalidArgumentException("Surname is too short");
      }
      if (name[0] >= 65 && name[0] <= 90) {
        throw InvalidArgumentException(
            "The first letter of a name can't be lowercase");
      }
      for (int i = 0; i < name.length(); i++) {
        if (!isalnum(name[i])) {
          throw InvalidArgumentException("Name contains invalid character");
        }
        _database.addUser(User(username, password, name, surname));
      };
    };
  }
};

class System {
public:
  static void Control() {
    Database db;
    Registration twitter(db);
  }
};
