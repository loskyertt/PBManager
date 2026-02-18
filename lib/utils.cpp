#define _CRT_SECURE_NO_WARNINGS

#include "utils.h"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>

using std::ifstream;
using std::ios;
using std::string;

constexpr int MAX_BUFF_SIZE = 4096;
constexpr int MAX_FIELD_SIZE = 128;

TelephoneDirect::TelephoneDirect() {
  ptr_init();
}

TelephoneDirect::TelephoneDirect(const string &path) {
  ptr_init();
  load(path);
}

void TelephoneDirect::load(const string &path) {
  size_t s_time = 0;
  ifstream in_file(path, ios::in);
  if (!in_file.is_open()) {
    std::cout << "路径下没有这个文件\n";
  }
  char buff[MAX_BUFF_SIZE];
  char name[MAX_FIELD_SIZE], city[MAX_FIELD_SIZE];
  char phone[MAX_FIELD_SIZE], hphone[MAX_FIELD_SIZE];
  char ophone[MAX_FIELD_SIZE], email[MAX_FIELD_SIZE];
  char company[MAX_FIELD_SIZE], addr[MAX_FIELD_SIZE];
  char bgroup[MAX_FIELD_SIZE], brief[MAX_FIELD_SIZE], time[MAX_FIELD_SIZE];
  while (in_file.getline(buff, MAX_BUFF_SIZE)) {
    s_time++;
    sscanf(buff, "%s %s %s %s %s %s %s %s %[^\n]\n", name, city, phone, hphone,
           ophone, email, company, addr, time);
    TelephoneItem item =
        TelephoneItem(string(name), string(city), string(phone), string(hphone),
                      string(ophone), string(email), string(company),
                      string(addr), string(time));
    this->insertToTelesNormal(item);
  }
  if (phoneMap.getFuzzyFunc() == nullptr) {}
}

void TelephoneDirect::ptr_init() {}

#define INSERT_TO_MAP(obj) obj##Map.insert(item.obj, item)

#define INSERT_TO_ARRAY_MAP(obj)        \
  if (obj##Map.find(item.obj)) {        \
    obj##Map[item.obj].push_back(item); \
  } else {                              \
    std::vector<TelephoneItem> arr;     \
    arr.push_back(item);                \
    obj##Map.insert(item.obj, arr);     \
  }

void TelephoneDirect::insertToTelesNormal(const TelephoneItem &item) {
  teles.push_back(item);
  INSERT_TO_MAP(name);
  INSERT_TO_ARRAY_MAP(city);
  INSERT_TO_MAP(phone);
  INSERT_TO_MAP(hphone);
  INSERT_TO_ARRAY_MAP(ophone);
  INSERT_TO_MAP(email);
  INSERT_TO_ARRAY_MAP(company);
  INSERT_TO_ARRAY_MAP(address);
  INSERT_TO_MAP(add_time);
  if (!item.belong_group.empty()) {
    INSERT_TO_ARRAY_MAP(belong_group);
  }
  if (!item.brief.empty()) {
    INSERT_TO_MAP(brief);
  }
}

#define ERASE_FROM_MAP(obj) obj##Map.remove(item.obj)

#define ERASE_FROM_ARRAY_MAP(obj)                                \
  if (obj##Map.find(item.obj)) {                                 \
    auto &arr = obj##Map[item.obj];                              \
    arr.erase(std::remove_if(arr.begin(), arr.end(),             \
                             [&](const TelephoneItem &in_item) { \
                               return item == in_item;           \
                             }),                                 \
              arr.end());                                        \
  }

void TelephoneDirect::eraseFromTelesNormal(const TelephoneItem &item) {
  auto NewEnd = std::remove(teles.begin(), teles.end(), item);
  teles.erase(NewEnd, teles.end());
  ERASE_FROM_MAP(name);
  ERASE_FROM_ARRAY_MAP(city);
  ERASE_FROM_MAP(phone);
  ERASE_FROM_MAP(hphone);
  ERASE_FROM_ARRAY_MAP(ophone);
  ERASE_FROM_MAP(email);
  ERASE_FROM_ARRAY_MAP(company);
  ERASE_FROM_ARRAY_MAP(address);
  ERASE_FROM_MAP(add_time);
}

bool operator==(const TelephoneItem &lhs, const TelephoneItem &rhs) {
  return lhs.name == rhs.name && lhs.city == rhs.city &&
         lhs.phone == rhs.phone && lhs.hphone == rhs.hphone &&
         lhs.ophone == rhs.ophone && lhs.email == rhs.email &&
         lhs.company == rhs.company && lhs.address == rhs.address &&
         lhs.belong_group == rhs.belong_group && lhs.brief == rhs.brief &&
         lhs.add_time == rhs.add_time;
}

std::ostream &operator<<(std::ostream &os, const TelephoneItem &item) {
  os << "姓名: " << item.name << " 城市: " << item.city
     << " 手机: " << item.phone << " 住宅电话: " << item.hphone
     << " 办公电话: " << item.ophone << " 电子邮件: " << item.email
     << " 公司: " << item.company << " 地址: " << item.address;

  if (!item.belong_group.empty()) {
    os << " 所属群组: " << item.belong_group;
  }

  if (!item.brief.empty()) {
    os << " 备注: " << item.brief;
  }

  os << " 添加时间: " << item.add_time << "\n";

  return os;
}

std::ostream &operator<<(std::ostream &os,
                         const std::vector<TelephoneItem> &items) {
  for (auto item : items) {
    os << item;
  }
  return os;
}