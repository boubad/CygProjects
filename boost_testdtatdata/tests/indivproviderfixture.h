/*
 * indivproviderfixture.h
 *
 *  Created on: 13 mai 2016
 *      Author: boubad
 */

#ifndef INDIVPROVIDERFIXTURE_H_
#define INDIVPROVIDERFIXTURE_H_
/////////////////////////////////
#include "databasefixture.h"
#include <indiv.h>
////////////////////////////////////
class IndivProviderFixture : public DatabaseFixture {
protected:
	std::unique_ptr<info::IIndivProvider> m_provider;
public:
	IndivProviderFixture ();
	virtual ~IndivProviderFixture();
public:
	void write_point(const info::DbValueMap &oMap,std::string &s);
	void write_indis_map(const info::ints_size_t_map &oMap, std::string &s);
};// class  DatabaseFixture
  //
///////////////////////////////////
#endif /* INDIVPROVIDERFIXTURE_H_ */
