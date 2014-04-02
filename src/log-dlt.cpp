#include "log-dlt.h"

namespace pelagicore {

void DltContextClass::registerContext() {

	if (!m_isInitialized) {
		if (!s_pAppLogContext->dltRegistered) {
			dlt_register_app( s_pAppLogContext->m_id.c_str(),
					  s_pAppLogContext->m_description.c_str() );
			s_pAppLogContext->dltRegistered = true;
		}
		//				auto code =
		dlt_register_context(this, m_contextID, m_description);
		//				assert(code == 0);
		m_isInitialized = true;
	}

}

}
