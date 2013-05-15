/*
* Copyright (C) 2013 Nivis LLC.
* Email:   opensource@nivis.com
* Website: http://www.nivis.com
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3 of the License.
* 
* Redistribution and use in source and binary forms must retain this
* copyright notice.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#ifndef MOCKKEYGENERATOR_H_
#define MOCKKEYGENERATOR_H_

#include "Common/logging.h"
#include "Model/SecurityKey.h"
#include "Security/KeyGenerator.h"

namespace Isa100 {
namespace Security {

/**
 * This is a Key Generator mock class.
 * Has only 1 key : "C0 C1 C2 C3 C4 C5 C6 C7 C8 C9 CA CB CC CD CE CF"
 * @author Ioan-Vasile Pocol
 * @version 1.0
 */
class MockKeyGenerator: public Isa100::Security::KeyGenerator {
        LOG_DEF("I.S.MockKeyGenerator")

    public:
        MockKeyGenerator();

        virtual ~MockKeyGenerator();

        virtual NE::Model::SecurityKey generateKey();

        virtual Uint32 getNextChallenge();

};
}
}
#endif /*MOCKKEYGENERATOR_H_*/
