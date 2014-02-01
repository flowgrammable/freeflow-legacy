namespace freeflow {

inline 
Read::Read(int f, uint8_t* b, int n) :
  fd_state(fd), buff(b), n_bytes(n), n_read(0)
  { }

inline int
Read::spin()
{
  while(n_read < n_bytes and fd_state > 0)
    *this();
  return int(*this);
}

inline int
Read::read_available()
{
  fd_state = ::read(fd, buff+n_read, n_bytes-n_read);
  if (fd_state > 0)
    n_read += fd_state;
  return fd_state < 0 ? fd_state : n_read;
}

inline int
Read::read_all()
{
  while(n_read < n and fd_state > 0) {
  fd_state = ::read(fd, buff+n_read, n_bytes-n_read);
  if (fd_state > 0)
    n_read += fd_state;
  }
  return fd_state < 0 ? fd_state : n_read;
}

inline int
Read::operator int()
{
  return fd_state < 0 ? fd_state : n_read;
}

inline 
Write::Write(int f, uint8_t* b, int n) :
  fd_state(fd), buff(b), n_bytes(n), n_written(0)
  { }

inline int
Write::spin()
{
  while(n_written < n_bytes and fd_state > 0)
    *this();
  return int(*this);
}

inline void
Write::operator()()
{
  fd_state = ::write(fd, buff+n_written, n_bytes-n_written);
  if (fd_state > 0)
    n_written += fd_state;
}

inline int
Write::operator int()
{
  return fd_state < 0 ? fd_state : n_written;
}

} // namespace freeflow
