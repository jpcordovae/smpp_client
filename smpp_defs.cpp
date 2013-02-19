#include "smpp_defs.hpp"

void dump_buffer(boost::uint8_t *_buffer, size_t _size)
{
   for (int k=0;k<_size;k++)
   {
      if( !(k%4) && k>1 )
      {
	 printf("\n");
      }
      
      printf("0x%.2X \'%-1c\'     ",_buffer[k],_buffer[k]);
   }
   printf("\n");
}
