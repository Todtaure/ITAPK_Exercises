/*****************************************/
/* Copyright: DevelEdu 2013              */
/* Author: sha                           */
/*****************************************/

#include <fstream>
#include <iostream>
#include <iterator>

#include <algorithm>
#include <numeric>
#include <functional>
#include <vector>
#include <string>

#include <boost/test/minimal.hpp>

#define PRODUCT_DB_FILE		"product.db"
#include "../MyArray.h"

class Product
{
public:
  Product(const std::string& name, float price, unsigned int sold = 0)
    : name_(name), price_(price), sold_(sold)
  {}
  Product()
    : name_(""), price_(0), sold_(0)
  {}
  
  const std::string& name() const
  {
    return name_;
  }
  
  float price() const
  {
    return price_;
  }

  void setPrice(float newPrice)
  {
    price_ = newPrice;
  }

  void setDiscount(float discount)
  {
	  price_ *= (1 - (discount / 100));
  }
  
  unsigned int sold() const
  {
    return sold_;
  }
  
  friend std::istream& operator>> ( std::istream& i, Product& p );
  friend std::ostream& operator<< ( std::ostream& o, const Product& p );
  
private:
  std::string   name_;
  float         price_;
  int           sold_;
};
typedef std::vector<Product>  ProductList;


std::ostream& operator<< ( std::ostream& o, const Product& p )
{ 
  return o << p.name_ << " " << p.price_ << " " << p.sold_; 
}

std::istream& operator>> ( std::istream& i, Product& p )
{
    return i >> p.name_ >> p.price_ >> p.sold_;
}



/**
 * Read products from file
 */
void productDBRead(ProductList& pl, const std::string& fileName)
{
  pl.clear();
  std::ifstream pFile( fileName.c_str() );

  std::istream_iterator<Product> eos;
  std::istream_iterator<Product> iit(pFile);

  while( iit != eos )
  {
    Product p;
	p = *iit;
    pl.push_back( p );
	++iit;
  }

  pFile.close();
}


/**
 * Printout all products
 */
void printAll(const ProductList& pl)
{
  std::cout << "##################################################" << std::endl;
  std::cout << "Printing out all products..." << std::endl;
  std::cout << "----------------------------" << std::endl;

  std::ostream_iterator<Product> oit(std::cout, "\n---\n");
  std::copy(pl.begin(), pl.end(), oit);

  /*for(ProductList::const_iterator iter = pl.begin(); iter != pl.end(); ++iter)
  {
    std::cout << *iter << std::endl;
  }  */
  std::cout << "##################################################" << std::endl;
}



/**
   Add item
*/
void addItem(ProductList& pl)
{
	Product newProd("Braendt", 5000.55f, 1);
	pl.push_back(newProd);
}


/**
   Write data to db file
*/
void productDBWrite(const ProductList& pl, const std::string& fileName)
{
	std::ofstream pFile(fileName.c_str());

	std::ostream_iterator<Product> oit(pFile,"\n");
	std::copy(pl.begin(), pl.end(), oit);
	pFile.close();
}


/**
 * Print poorly selling products
 */
void printPoorlySellingProducts(const ProductList& pl)
{
	std::cout << "##################################################" << std::endl;
	std::cout << "Printing out poorly sold products..." << std::endl;
	std::cout << "----------------------------" << std::endl;

	std::ostream_iterator<Product> oit(std::cout, "\n---\n");
	std::remove_copy_if(pl.begin(), pl.end(), oit, 
		[](Product p)
	{
		return (p.sold() > 9);
	});

	std::cout << "##################################################" << std::endl;
}


/**
 * Set a discount on all products - Using for_each()
 */

struct DiscountForEach
{
	DiscountForEach(float discount) : discount_(discount)
	{
	}

	void operator () (Product &p)
	{
		auto perc = (1 - (discount_ / 100));
		p.setPrice(p.price() * perc);
	}

private:
	float discount_;
};

void addDiscountUsingForEach(ProductList& pl)
{
	std::for_each(pl.begin(), pl.end(), DiscountForEach(20));
}


/**
 * Set a discount on all products - Using transform()
 */
struct DiscountTrans
{
	DiscountTrans(float discount) : discount_(discount)
	{
	}

	Product operator ()(Product p)
	{
		auto perc = (1 - (discount_ / 100));
		p.setPrice(p.price() * perc);
		return p;
	}

private:
	float discount_;
};
void addDiscountUsingTransform(ProductList& pl)
{
	std::cout << "##################################################" << std::endl;
	std::ostream_iterator<Product> oit(std::cout, "\n---\n");
	std::transform(pl.begin(), pl.end(), oit, DiscountTrans(50));
}


/**
 * Calculate the total amount of sold products
 */
void calcTotalSoldProducts(ProductList& pl)
{
	std::vector<unsigned int> tmpSold = {};	
	std::transform(pl.begin(), pl.end(), std::back_inserter(tmpSold), std::mem_fun_ref<unsigned int, Product>(&Product::sold));
	
	std::cout << "##################################################" << std::endl;
	std::cout << "Products sold total: " << std::accumulate(tmpSold.begin(), tmpSold.end(), 0) << std::endl;
	std::cout << "##################################################" << std::endl;
}
//Transform is a little harder to understand and read than eg. for_each, especially when using the mem_fun_ref method.

/**
Apply Product discount using lambda function.
*/
void setProductDiscountLambda(ProductList &pl)
{
	std::for_each(pl.begin(), pl.end(), 
		[](Product &p)
	{
		p.setDiscount(25);
	});

	std::cout << "##################################################" << std::endl;
	std::cout << "Product discount applied." << std::endl;
	std::cout << "##################################################" << std::endl;
}


/**
 * Setting discount using bind2nd - OPTIONAL
 */
void testMyArrayIterator()
{
	MyArray<int> arr;
	arr.fill(3);

	BOOST_CHECK(std::accumulate(arr.begin(), arr.end(), 0) == 30);

	std::for_each(arr.begin(), arr.end(), [](int &i) {i *= 2; });

	BOOST_CHECK(arr[0] == 6);
	BOOST_CHECK(std::accumulate(arr.begin(), arr.end(), 0) == 60);

	std::ostream_iterator<int> oit(std::cout, ", ");
	std::cout << "MyArray content: ";
	std::copy(arr.begin(), arr.end(), oit);
	std::cout << std::endl;
}

/**
Test methods for Custom iterator for MyArray
*/

int test_main(int argc, char* argv[])
{
  bool        running = true;
  ProductList pl;
  
  while(running)
  {
    char choice;
    
    std::cout << "********************" << std::endl;
    std::cout << "Help menu: " << std::endl;
    std::cout << "'1' Read product database" << std::endl;
    std::cout << "'2' Print all items" << std::endl;
    std::cout << "'3' Add item" << std::endl;
    std::cout << "'4' Write product database" << std::endl;
    std::cout << "'5' Print poorly selling products" << std::endl;
    std::cout << "'6' Set a discount on all products (using for_each() )" << std::endl;
    std::cout << "'7' Set a discount on all products (using transform() )" << std::endl;
    std::cout << "'8' Calculate the total amount of sold products" << std::endl;
	std::cout << "'9' Set a discount on all products (using lambda )" << std::endl;

    
    std::cout << "'q' Quit" << std::endl;
    std::cout << "Your choice: ";
    std::cin >> choice;
    
    switch(choice)
    {
      case '1':
        productDBRead(pl, PRODUCT_DB_FILE);
        break;

      case '2':
        printAll(pl);
        break;

      case '3':
        addItem(pl);
        break;

      case '4':
        productDBWrite(pl, PRODUCT_DB_FILE);
        break;

      case '5':
        printPoorlySellingProducts(pl);
        break;
        
      case '6':
        addDiscountUsingForEach(pl);
        break;

      case '7':
        addDiscountUsingTransform(pl);
        break;

      case '8':
        calcTotalSoldProducts(pl);
        break;

	  case '9':
		setProductDiscountLambda(pl);
		break;

	  case 'x':
		  testMyArrayIterator();
		  break;
        
      case 'q':
      case 'Q':
        running = false;
    }
  }

  return 0;
}