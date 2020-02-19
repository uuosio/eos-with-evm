/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#include <sstream>

void prints( const char* cstr ) {
//   wlog("++++++${n}", ("n", cstr));
   if (!get_ctx_no_access_check().control.contracts_console()) {
      return;
   }
   get_ctx_no_access_check().console_append(cstr);
}

void prints_l( const char* cstr, uint32_t len) {
//   wlog("++++++${n}", ("n", cstr));
   if (!get_ctx_no_access_check().control.contracts_console()) {
      return;
   }
   get_ctx_no_access_check().console_append(string(cstr, len));
}

void printi( int64_t val ) {
//   wlog("++++++${n}", ("n", val));
   if (!get_ctx_no_access_check().control.contracts_console()) {
      return;
   }
   std::ostringstream oss;
   oss << val;
   get_ctx_no_access_check().console_append( oss.str() );
}

void printui( uint64_t val ) {
//   wlog("++++++${n}", ("n", val));
   if (!ctx().control.contracts_console()) {
      return;
   }
   std::ostringstream oss;
   oss << val;
   get_ctx_no_access_check().console_append( oss.str() );
}

void printi128( const int128_t* val ) {
//   wlog("++++++${n}", ("n", (uint64_t)val));
   if (!get_ctx_no_access_check().control.contracts_console()) {
      return;
   }
   bool is_negative = (*val < 0);
   unsigned __int128 val_magnitude;

   if( is_negative )
      val_magnitude = static_cast<unsigned __int128>(-*val); // Works even if val is at the lowest possible value of a int128_t
   else
      val_magnitude = static_cast<unsigned __int128>(*val);

   fc::uint128_t v(val_magnitude>>64, static_cast<uint64_t>(val_magnitude) );

   if( is_negative ) {
      get_ctx_no_access_check().console_append("-");
   }

   get_ctx_no_access_check().console_append(fc::variant(v).get_string());
}

void printui128( const uint128_t* val ) {
//   wlog("++++++${n}", ("n", (uint64_t)val));
   if (!get_ctx_no_access_check().control.contracts_console()) {
      return;
   }
   fc::uint128_t v(*val>>64, static_cast<uint64_t>(*val) );
   get_ctx_no_access_check().console_append(fc::variant(v).get_string());
}

void printsf(float val) {
   if (!get_ctx_no_access_check().control.contracts_console()) {
      return;
   }
   std::ostringstream oss;
   oss.setf( std::ios::scientific, std::ios::floatfield );
   oss.precision( std::numeric_limits<float>::digits10 );
   oss << val;
   get_ctx_no_access_check().console_append( oss.str() );
}

void printdf(double val) {
//   wlog("++++++${n}", ("n", val));
   if (!ctx().control.contracts_console()) {
      return;
   }
   std::ostringstream oss;
   oss.setf( std::ios::scientific, std::ios::floatfield );
   oss.precision( std::numeric_limits<double>::digits10 );
   oss << val;
   get_ctx_no_access_check().console_append( oss.str() );
}

void printqf(const float128_t* val) {
//   wlog("++++++${n}", ("n", (uint64_t)val));
   if (!get_ctx_no_access_check().control.contracts_console()) {
      return;
   }
   std::ostringstream oss;
   oss.setf( std::ios::scientific, std::ios::floatfield );

#ifdef __x86_64__
   oss.precision( std::numeric_limits<long double>::digits10 );
   extFloat80_t val_approx;
   f128M_to_extF80M(val, &val_approx);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
   oss << *(long double*)(&val_approx);
#pragma GCC diagnostic pop
#else
   oss.precision( std::numeric_limits<double>::digits10 );
   double val_approx = from_softfloat64( f128M_to_f64(val) );
   oss << val_approx;
#endif
   get_ctx_no_access_check().console_append( oss.str() );
}

void printn( uint64_t n ) {
//   wlog("++++++${n}", ("n", n));
   if (!get_ctx_no_access_check().control.contracts_console()) {
      return;
   }
   get_ctx_no_access_check().console_append(name(n).to_string());
}

void printhex( const void* data, uint32_t datalen ) {
//   wlog("++++++${n}", ("n", (uint64_t)data));
   if (!get_ctx_no_access_check().control.contracts_console()) {
      return;
   }
   get_ctx_no_access_check().console_append(fc::to_hex((char*)data, datalen));
}

