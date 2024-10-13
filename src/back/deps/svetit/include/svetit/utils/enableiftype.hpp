#pragma once

namespace svetit::utils {

template<class T, class R = void>
struct enable_if_type { typedef R type; };

} // namespace svetit::utils
