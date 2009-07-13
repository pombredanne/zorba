/*
 * Copyright 2006-2008 The FLWOR Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

%module zorba_api
%include "std_string.i"
%include "exception.i"

%exception {
  try {
    $action
  }
  catch (zorba::QueryException& e) {
    std::stringstream lStream;
    lStream << e;
    SWIG_exception(SWIG_RuntimeError, lStream.str().c_str());
  }
  catch (zorba::ZorbaException& e) {
    std::stringstream lStream;
    lStream << e;
    SWIG_exception(SWIG_RuntimeError, lStream.str().c_str());
  }
}

%{
#include <zorba/zorba.h>
#include <simplestore/simplestore.h>
#include <string>
#include <sstream>
#include <iostream>

class Item {
  friend class ResultIterator;
private:
  zorba::Item theItem;
public:
  Item() {}
  Item(const Item& aItem) : theItem(aItem.theItem) {}
  static Item createEmptyItem() { return Item(); }
  std::string getStringValue() const { return 		std::string(theItem.getStringValue().c_str()); }

  std::string serialize() const {
    std::stringstream lStream;
    theItem.serialize(lStream);
    return lStream.str();
  }


}; // class Item

class ResultIterator {
private:
  zorba::ResultIterator_t theResultIterator;
public:
  ResultIterator() {}
  ResultIterator(const ResultIterator& aResultIterator) 
  : theResultIterator(aResultIterator.theResultIterator) 
  {}
  ResultIterator(zorba::ResultIterator_t aResultIterator) : theResultIterator(aResultIterator) {}
  void open() { theResultIterator->open(); }
  bool next(Item& aItem) 
  { 
      return theResultIterator->next(aItem.theItem); 
  }
  void close() { theResultIterator->close(); }
  void destroy() { theResultIterator = 0; }

}; // class ResultIterator

class XQuery {
private:
  zorba::XQuery_t theQuery;
public:
  XQuery() {}
  XQuery(const XQuery& aXQuery) : theQuery(aXQuery.theQuery) {}
  XQuery(zorba::XQuery_t aQuery) : theQuery(aQuery) {}

  std::string execute()
  {
    std::stringstream lStream;
      lStream << theQuery;
    return lStream.str();
  }

  void setVariableAsDocument(const std::string& aVarName, const std::string& aURL, const std::string& aContent){
   std::auto_ptr<std::istream> lDocStream(new std::stringstream(aContent));
  	zorba::DynamicContext* lCtx = theQuery->getDynamicContext();

  	lCtx->setVariableAsDocument(aVarName, aURL, lDocStream);
   }

  std::string printPlanAsXML()
  {
    std::ostringstream lStream;
    theQuery->printPlan(lStream);
    return lStream.str();
  }

  std::string printPlanAsDOT()
  {
    std::ostringstream lStream;
    theQuery->printPlan(lStream, true);
    return lStream.str();
  }

#ifdef SWIGPYTHON
  void executeSAX(SAX2ContentHandlerProxy* contentHandlerProxy)
  {
    theQuery->executeSAX(contentHandlerProxy);
  }
#endif

  bool isUpdateQuery() { return theQuery->isUpdateQuery(); }
  void applyUpdates() { theQuery->applyUpdates(); }
  void destroy() { theQuery = 0; }
  ResultIterator iterator() { return ResultIterator(theQuery->iterator()); }
}; // class XQuery

class Store {
public:
  Store() {}
  Store(const Store&) {}
  virtual ~Store() {}
  virtual zorba::simplestore::SimpleStore* getStore() const { return 0; }
  // TODO the above line must be replace by the following line!!
  // virtual zorba::simplestore::Store* getStore() const { return 0; }
};

class InMemoryStore : public Store {
private:
  zorba::simplestore::SimpleStore* theStore;
public:
  InMemoryStore() : theStore(0) {}
  InMemoryStore(const InMemoryStore& aStore) : Store(aStore), theStore(aStore.theStore) {}
  virtual ~InMemoryStore() {}
  InMemoryStore(zorba::simplestore::SimpleStore* aStore) : theStore(aStore) {}
  static InMemoryStore getInstance() { return InMemoryStore(zorba::simplestore::SimpleStoreManager::getStore());
  }
  InMemoryStore& operator=(const InMemoryStore& aStore) 
  {
    theStore = aStore.theStore; 
    return *this;
  }
  static void shutdown(InMemoryStore& aStore)
  {
    zorba::simplestore::SimpleStoreManager::shutdownStore(aStore.theStore);
  }
  virtual zorba::simplestore::SimpleStore* getStore() const
  {
    return theStore;
  }
};

class XmlDataManager {
private:
  zorba::XmlDataManager* theManager;
public:
  XmlDataManager() : theManager(0) {}
  XmlDataManager(const XmlDataManager& aManager) : theManager(aManager.theManager) {} 
  XmlDataManager(zorba::XmlDataManager* aManager) : theManager(aManager) {}
  void loadDocument(const std::string& aName, const std::string& aContent)
  {
    std::stringstream lStream(aContent);
    theManager->loadDocument(aName, lStream);
  }
  bool deleteDocument(const std::string& aName) { return theManager->deleteDocument(aName); }
};

class Zorba {
private:
  zorba::Zorba* theZorba;
  Zorba(zorba::Zorba* aZorba):theZorba(aZorba){}
public:
  Zorba():theZorba(0){}
  Zorba(const Zorba& aZorba) : theZorba(aZorba.theZorba) {}
  static Zorba getInstance(const Store& aStore)
  {
    return Zorba(zorba::Zorba::getInstance(aStore.getStore()));
  }
  XQuery compileQuery(const std::string& aStr) 
  {
      return XQuery(theZorba->compileQuery(aStr));
  }
  XmlDataManager getXmlDataManager()
  {
    return XmlDataManager(theZorba->getXmlDataManager());
  }
  void shutdown() { theZorba->shutdown(); }
}; // class Zorba


%}

class Item {
public: 
  static Item createEmptyItem();
  std::string getStringValue() const;
  std::string serialize() const;
};

class ResultIterator {
public:
  void open();
  bool next(Item&);
  void close();
  void destroy();
};

class XQuery {
public:
  std::string execute();
  std::string printPlanAsXML();
  std::string printPlanAsDOT();

#ifdef SWIGPYTHON
  void executeSAX(SAX2ContentHandlerProxy* contentHandlerProxy);
#endif

  bool isUpdateQuery();
  void applyUpdates();
  void destroy();
  ResultIterator iterator();
  void setVariableAsDocument(const std::string& aVarName, const std::string& aURL, const std::string& aContent);
};

class Store {};

class InMemoryStore : public Store {
public:
  static InMemoryStore getInstance();
  static void shutdown(InMemoryStore&);
};

class XmlDataManager {
public:
  void loadDocument(const std::string& aName, const std::string& aContent);
  bool deleteDocument(const std::string& aName);
};

class Zorba {
public:
  static Zorba getInstance(const Store&);
  XQuery compileQuery(const std::string& aStr);
  XmlDataManager getXmlDataManager();
  void shutdown();
};
