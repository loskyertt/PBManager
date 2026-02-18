#pragma once
#ifndef __PM_H__
#define __PM_H__

#include <deque>
#include <vector>

#include "algori.hpp"

struct TelephoneItem {
  typedef std::string string;
  string name, city, phone, hphone;
  string ophone, email, company, address;
  string belong_group, brief, add_time;

  TelephoneItem() = default;

  TelephoneItem(const string &name, const string &city, const string &p,
                const string &p1, const string &p2, const string &email,
                const string &company, const string &addr, const string &__time)
      : name(name),
        city(city),
        phone(p),
        hphone(p1),
        ophone(p2),
        email(email),
        company(company),
        address(addr),
        add_time(__time) {}

  TelephoneItem(const string &name, const string &city, const string &phone,
                const string &hphone, const string &ophone, const string &email,
                const string &company, const string &address,
                const string &belong_group, const string &brief,
                const string &time)
      : name(name),
        city(city),
        phone(phone),
        hphone(hphone),
        ophone(ophone),
        email(email),
        company(company),
        address(address),
        belong_group(belong_group),
        brief(brief),
        add_time(add_time) {}

  TelephoneItem(const TelephoneItem &other)
      : name(other.name),
        city(other.city),
        phone(other.phone),
        hphone(other.hphone),
        ophone(other.ophone),
        email(other.email),
        company(other.company),
        address(other.address),
        belong_group(other.belong_group),
        brief(other.brief),
        add_time(other.add_time) {}

  friend bool operator==(const TelephoneItem &item1,
                         const TelephoneItem &item2);

  friend std::ostream &operator<<(std::ostream &os, const TelephoneItem &item);

  friend std::ostream &operator<<(std::ostream &os,
                                  const std::vector<TelephoneItem> &items);
};

typedef std::function<bool(const std::string &k1, const std::string &k2)>
    cmp_ptr;

#define TELEPHONE_IF_FIND(FUNC_NAME, PARAMS_NAME)                 \
  inline bool findIf##FUNC_NAME(const std::string &PARAMS_NAME) { \
    return PARAMS_NAME##Map.find(PARAMS_NAME);                    \
  }

#define TELEPHONE_FIND_API(FUNC_NAME, PARAMS_NAME)                             \
  inline TelephoneItem findItemBy##FUNC_NAME(const std::string &PARAMS_NAME) { \
    return PARAMS_NAME##Map[PARAMS_NAME];                                      \
  }

#define TELEPHONES_FIND_API(FUNC_NAME, PARAMS_NAME)        \
  inline std::vector<TelephoneItem> findItemBy##FUNC_NAME( \
      const std::string &PARAMS_NAME) {                    \
    return PARAMS_NAME##Map[PARAMS_NAME];                  \
  }

#define TELEPHONE_FUZZY_API(FUNC_NAME, PARAMS_NAME)         \
  inline std::vector<TelephoneItem> findFuzzy##FUNC_NAME(   \
      const std::string &PARAMS_NAME, const cmp_ptr &ptr) { \
    return PARAMS_NAME##Map.findValues(PARAMS_NAME, ptr);   \
  }

class TelephoneDirect {
 public:
  TelephoneDirect();
  explicit TelephoneDirect(const std::string &path);
  bool has_brief = false, has_group = false;

 public:
  void ptr_init();
  std::deque<TelephoneItem> teles;
  AVLTree<std::string, TelephoneItem> nameMap, phoneMap, hphoneMap;
  AVLTree<std::string, TelephoneItem> emailMap, briefMap, add_timeMap;
  AVLTree<std::string, std::vector<TelephoneItem>> ophoneMap, companyMap,
      addressMap, belong_groupMap, cityMap;

 public:
  void load(const std::string &path);
  void insertToTelesNormal(const TelephoneItem &item);
  void eraseFromTelesNormal(const TelephoneItem &item);
  TelephoneItem nextToTeles();
  TELEPHONE_IF_FIND(Name, name);
  TELEPHONE_IF_FIND(City, city);
  TELEPHONE_IF_FIND(Phone, phone);
  TELEPHONE_IF_FIND(Brief, brief);
  TELEPHONE_IF_FIND(Email, email);
  TELEPHONE_IF_FIND(Address, address);
  TELEPHONE_IF_FIND(Company, company);
  TELEPHONE_IF_FIND(HomePhone, hphone);
  TELEPHONE_IF_FIND(AddTime, add_time);
  TELEPHONE_IF_FIND(OfficePhone, ophone);
  TELEPHONE_IF_FIND(BelongGroup, belong_group);

  TELEPHONE_FIND_API(Name, name);
  TELEPHONE_FUZZY_API(Name, name);
  TELEPHONES_FIND_API(City, city);
  TELEPHONE_FUZZY_API(Phone, phone);
  TELEPHONE_FIND_API(Brief, brief);
  TELEPHONE_FIND_API(Email, email);
  TELEPHONES_FIND_API(Address, address);
  TELEPHONES_FIND_API(Company, company);
  TELEPHONE_FUZZY_API(HomePhone, hphone);
  TELEPHONE_FIND_API(AddTime, add_time);
  TELEPHONES_FIND_API(BelongGroup, belong_group);

  inline std::vector<TelephoneItem> findFuzzyOfficePhone(
      const std::string &ophone, const cmp_ptr &ptr) {
    auto arrs = ophoneMap.findValues(ophone, ptr);
    std::vector<TelephoneItem> items;
    for (auto arr : arrs) {
      for (auto item : arr) {
        items.push_back(item);
      }
    }
    return items;
  }

  size_t length() { return nameMap.length(); }
};

#undef TELEPHONE_FIND_API

#endif
