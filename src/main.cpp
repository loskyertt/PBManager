#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <functional>
#include <map>
#include <regex>

#include "utils.h"

using namespace std;

TelephoneDirect direct;

vector<string> paths = {"./data/5000.txt", "./data/10000.txt",
                        "./data/100000.txt"};

void loadCharMap();

int SystemInitWidget();

void AddTelephoneItem();
void DelTelephoneItem();
void EditTelephoneItem();

void AddGroupItem();
void DelGroupItem();
void EditGroupItem();

void ImportTeleItems();
void ExportTeleItems();

void FindTelephoneItem();
void ShowAllTelephoneItems();

int main(int argc, char **argv) {
  int choice;
  loadCharMap();
  cout << "===================================\n";
  cout << "\n选择想要导入的电话簿(1. 5000.txt 2. 10000.txt 3. 100000.txt)\n";
  cin >> choice;
  if (choice >= 1 && choice <= 3) {
    direct.load(paths[choice - 1]);
    cout << "导入数据: " << direct.length() << "条\n";
  } else {
    cout << "无效的输入\n";
    return 0;
  }
  SystemInitWidget();
  return 0;
}

struct helper {
  static std::vector<std::string> groups;
  static std::map<std::string,
                  std::function<void(TelephoneItem &, const std::string &)>>
      setters;
  static function<bool(const std::string &k1, const std::string &k2)>
      contain_ptr;
  static function<bool(const std::string &k1, const std::string &k2)>
      name_contain_ptr;
  static std::map<std::string, std::string> chars_map;
};

std::vector<std::string> helper::groups;
std::map<std::string, std::string> helper::chars_map;

cmp_ptr helper::contain_ptr = [](const std::string &k1, const std::string &k2) {
  return k1.find(k2) != std::string::npos;
};

cmp_ptr helper::name_contain_ptr = [](const std::string &k1,
                                      const std::string &k2) {
  bool cmp1 = (k1.find(k2) != std::string::npos);
  string k1pinyin, k1jianpin, k2pinyin, k2jianpin;
  for (size_t i = 0; i < k1.size(); i += 2) {
    std::string substring = k1.substr(i, 2);
    string map_str = helper::chars_map[substring];
    for (auto c : map_str)
      k1pinyin.push_back(c);
    k1jianpin.push_back(map_str[0]);
  }
  int k2_is_engish = 0;
  for (int i = 0; i < k2.length(); i++)
    if ((k2[i] >= 'a' && k2[i] <= 'z') || k2[i] >= 'A' && k2[i] <= 'Z')
      k2_is_engish = 1;
  if (k2_is_engish) {
    k2pinyin = k2, k2jianpin = k2;
  } else {
    for (size_t i = 0; i < k2.size(); i += 2) {
      std::string substring = k2.substr(i, 2);
      string map_str = helper::chars_map[substring];
      for (auto c : map_str)
        k2pinyin.push_back(c);
      k2jianpin.push_back(map_str[0]);
    }
  }
  bool cmp3 = (k1pinyin.find(k2pinyin) != string::npos);
  bool cmp2 = (k1jianpin.find(k2jianpin) != string::npos);
  return (cmp1 || cmp2 || cmp3);
};

void loadCharMap() {
  ifstream infile("./pinyin.txt");
  if (!infile.is_open()) {
    std::cerr << "文件打开失败\n";
    return;
  }

  std::regex pattern(R"(U\+([0-9A-F]+):\s+([^\s]+)\s+#\s+([^\n]+))");
  std::smatch match;

  std::string line;
  while (std::getline(infile, line)) {
    if (std::regex_match(line, match, pattern)) {
      std::string unicode = match[1];
      std::string pinyin = match[2];
      std::string character = match[3];
      std::string ch_zh;
      for (int i = 0; i < 2; i++)
        ch_zh.push_back(character[i]);
      helper::chars_map.insert(pair<string, string>{ch_zh, pinyin});
    }
  }
  infile.close();
}

int SystemInitWidget() {
  int choice;

  do {
    cout << "\n================================================================"
            "======\n";
    cout << "=================================================================="
            "====\n";
    cout << "***************************  联系人管理系统  "
            "**************************";
    cout << "\n================================================================"
            "======";
    cout << "\n================================================================"
            "======\n";

    cout << "1. 添加联系人\n";
    cout << "2. 删除联系人\n";
    cout << "3. 编辑联系人\n";
    cout << "4. 添加群组\n";
    cout << "5. 删除群组\n";
    cout << "6. 编辑群组\n";
    cout << "7. 导入联系人\n";
    cout << "8. 导出联系人\n";
    cout << "9. 显示所有联系人\n";
    cout << "10. 查询联系人\n";
    cout << "11. 退出\n";

    cout << "请选择操作: ";
    cin >> choice;

    switch (choice) {
      case 1:
        AddTelephoneItem();
        break;
      case 2:
        DelTelephoneItem();
        break;
      case 3:
        EditTelephoneItem();
        break;
      case 4:
        AddGroupItem();
        break;
      case 5:
        DelGroupItem();
        break;
      case 6:
        EditGroupItem();
        break;
      case 7:
        ImportTeleItems();
        break;
      case 8:
        ExportTeleItems();
        break;
      case 9:
        ShowAllTelephoneItems();
        break;
      case 10:
        FindTelephoneItem();
        break;
      case 11:
        cout << "谢谢使用，再见！\n";
        break;
      default:
        cout << "无效选择，请重新输入。\n";
    }
  } while (choice != 11);

  return 0;
}

void AddTelephoneItem() {
  TelephoneItem item;
  cout << "请输入用户信息，请按照括号内的方式写入\n"
       << "(姓名 城市 手机号码 住宅电话号码 办公电话号码 电子邮件 公司 地址 "
          "添加时间)";
  cin >> item.name >> item.city >> item.phone >> item.hphone >> item.ophone >>
      item.email >> item.company >> item.address;
  getline(std::cin, item.add_time);
  cout << "是否需要添加备注和群组(Y或者N)";
  char choice;
  cin >> choice;
  if (choice == 'Y' || choice == 'y') {
    cin >> item.brief >> item.belong_group;
  }
  direct.insertToTelesNormal(item);
}

void DelTelephoneItem() {
  string name;
  cout << "请输入用户名字: ";
  cin >> name;
  if (direct.findIfName(name)) {
    auto item = direct.findItemByName(name);
    direct.eraseFromTelesNormal(item);
  } else {
    cout << "没有用户信息\n";
  }
}

std::map<std::string, std::function<void(TelephoneItem &, const std::string &)>>
    helper::setters = {
        {"name", [](TelephoneItem &item,
                    const std::string &value) { item.name = value; }},
        {"city", [](TelephoneItem &item,
                    const std::string &value) { item.city = value; }},
        {"phone", [](TelephoneItem &item,
                     const std::string &value) { item.phone = value; }},
        {"hphone", [](TelephoneItem &item,
                      const std::string &value) { item.hphone = value; }},
        {"email", [](TelephoneItem &item,
                     const std::string &value) { item.email = value; }},
        {"add_time", [](TelephoneItem &item,
                        const std::string &value) { item.add_time = value; }},
        {"ophone", [](TelephoneItem &item,
                      const std::string &value) { item.ophone = value; }},
        {"company", [](TelephoneItem &item,
                       const std::string &value) { item.company = value; }},
        {"address", [](TelephoneItem &item,
                       const std::string &value) { item.address = value; }},
};

#define EDIT_MAP(obj, value)         \
  direct.eraseFromTelesNormal(item); \
  helper::setters[obj](item, value); \
  direct.insertToTelesNormal(item);

void EditTelephoneItem() {
  string name, value;
  cout << "请输入用户名字: ";
  cin >> name;
  if (direct.findIfName(name)) {
    int choice;
    TelephoneItem item = direct.findItemByName(name);
    cout << "选择想要修改的用户信息: (1. 用户姓名 2. 城市 3. 手机号码 4. "
            "住宅电话 5. 办公电话 6. 电子邮件 7. 公司 8. 地址 9. 添加时间)";
    std::vector<std::string> choice_strs{
        "name",  "city",    "phone",   "hphone",   "ophone",
        "email", "company", "address", "add_time",
    };
    cin >> choice;
    if (choice >= 1 && choice <= 9) {
      cout << "请输入\n";
      cin >> value;
      string key = choice_strs[choice - 1];
      EDIT_MAP(key, value);
    } else {
      cout << "无效输入\n";
    }
  } else {
    cout << "没有用户信息\n";
  }
}

void ImportTeleItems() {
  string path;
  cout << "请输入文件导入路径: ";
  cin >> path;
  direct.load(path);
}

void ExportTeleItems() {
  string path;
  cout << "请输入文件导出路径: ";
  cin >> path;
  ofstream out_file(path);
  if (!out_file.is_open()) {
    cout << "文件打开失败\n";
    return;
  }
  char buff[2048];
  for (auto item : direct.teles) {
    sprintf(buff, "%s %s %s %s %s %s", item.name.c_str(), item.city.c_str(),
            item.phone.c_str(), item.hphone.c_str(), item.ophone.c_str(),
            item.email.c_str(), item.company.c_str(), item.address.c_str(),
            item.add_time.c_str());
    out_file.write(buff, sizeof(buff));
  }
}

void AddGroupItem() {
  std::string group;
  cout << "请输入群组名称: ";
  cin >> group;
  helper::groups.push_back(group);
  cout << "群组添加成功\n";
}

void DelGroupItem() {
  cout << "群组一共有: \n";
  for (int i = 0; i < helper::groups.size(); i++) {
    cout << "群组编号为: " << i + 1 << " 群组名称: " << helper::groups[i]
         << endl;
  }
  int choice;
  cout << "选择想要删除的编号: ";
  cin >> choice;
  if (choice >= 1 && choice <= helper::groups.size()) {
    helper::groups.erase(
        std::remove_if(helper::groups.begin(), helper::groups.end(),
                       [&](const std::string &group) {
                         return group == helper::groups[choice - 1];
                       }),
        helper::groups.end());

  } else {
    cout << "无效的编号\n";
  }
}

void EditGroupItem() {
  cout << "群组一共有: \n";
  std::string name;
  for (int i = 0; i < helper::groups.size(); i++) {
    cout << "群组编号为: " << i + 1 << " 群组名称: " << helper::groups[i]
         << endl;
  }
  int choice;
  cout << "请输入想要编辑的群组编号: ";
  cin >> choice;
  if (choice >= 1 && choice <= helper::groups.size()) {
    cout << "请输入想要编辑人员的姓名: ";
    cin >> name;
    if (!direct.findIfName(name)) {
      cout << "无此人员\n";
    } else {
    }
  } else {
    cout << "无效的编号\n";
  }
}

// 模糊搜索实现
void FindTelephoneItem() {
  int choice;
  string key;
  cout << "\n选择一种方式进行查询\n";
  cout << "\n1. 城市\n2. 时间\n3. 地址\n4. 电子邮件\n5. 手机号码\n";
  cout << "6. 住宅电话号码\n7. 办公电话号码\n8. 公司\n";
  cout << "9. 姓名\n10. 所属群组\n11. 备注\n";
  cin >> choice;
  if (choice >= 1 && choice <= 11) {
    cout << "输入信息: ";
    cin >> key;
    switch (choice) {
      case 1: {
        if (direct.findIfCity(key))
          cout << direct.findItemByCity(key) << endl;
        else
          cout << "没有查找到信息\n";
        break;
      }
      case 2: {
        if (direct.findIfAddTime(key))
          cout << direct.findItemByAddTime(key) << endl;
        else
          cout << "没有查找到信息\n";
        break;
      }
      case 3: {
        if (direct.findIfAddress(key))
          cout << direct.findItemByAddress(key) << endl;
        else
          cout << "没有查找到信息\n";
        break;
      }
      case 4: {
        if (direct.findIfEmail(key))
          cout << direct.findItemByEmail(key) << endl;
        else
          cout << "没有查找到信息\n";
        break;
      }
      case 5: {
        auto phoneArr = direct.findFuzzyPhone(key, helper::contain_ptr);
        if (phoneArr.size())
          cout << phoneArr << endl;
        else
          cout << "没有查找到信息\n";
        break;
      }
      case 6: {
        std::vector<TelephoneItem> homePhoneArr =
            direct.findFuzzyHomePhone(key, helper::contain_ptr);
        if (homePhoneArr.size())
          cout << homePhoneArr << endl;
        else
          cout << "没有查找到信息\n";
        break;
      }
      case 7: {
        auto officePhoneArr =
            direct.findFuzzyOfficePhone(key, helper::contain_ptr);
        if (officePhoneArr.size())
          cout << officePhoneArr << endl;
        else
          cout << "没有查找到信息\n";
        break;
      }
      case 8: {
        if (direct.findIfCompany(key))
          cout << direct.findItemByCompany(key) << endl;
        else
          cout << "没有查找到信息\n";
        break;
      }
      case 9: {
        auto nameArr2 = direct.findFuzzyName(key, helper::name_contain_ptr);
        if (nameArr2.size())
          cout << nameArr2 << endl;
        else
          cout << "没有查找到信息\n";
        break;
      }
      case 10: {
        if (!direct.findIfBelongGroup(key)) {
          cout << "尚无群组\n";
        } else {
          cout << direct.findItemByBelongGroup(key) << endl;
        }
        break;
      }
      case 11: {
        if (!direct.findIfBrief(key)) {
          cout << "尚无备注\n";
        } else {
          cout << direct.findItemByBrief(key) << endl;
        }
        break;
      }
    }
  } else {
    cout << "非法输入\n";
  }
}

void ShowAllTelephoneItems() {
  size_t idx = 0;
  char input;
  while (true) {
    for (size_t i = idx; i < idx + 5; i++) {
      cout << direct.teles[i];
    }
    std::cout << "请输入'i'获取更多信息, 'q'退出: ";
    std::cin >> input;

    if (input == 'i') {
      idx += 5;
    } else if (input == 'q') {
      break;
    } else {
      std::cin.clear();
    }
  }
}
