namespace freeflow {

inline
Transfer(int f, uint8_t* b, int n) :
  fd_state(fd), buff(b), n_bytes(n), n_transferred(0)
{ }

inline int
Transfer::operator int()
{
  return fd_state < 0 ? fd_state : n_written;
}

inline int
transfer_available(Transfer& t, int (*op)(int, uint8_t*, int))
{
  // Read the remainder to the buff
  fd_state = op(fd, buff+n_transferred, n_bytes-n_transferred);
  // If successful update bytes read
  if (fd_state > 0)
    n_transferred += fd_state;
  // Return fd state on failure or number of bytes read
  return fd_state < 0 ? fd_state : n_transferred;
}

inline int
transfer_all(Transfer& t, int (*op)(int, uint8_t*, int))
{
  // More to read and fd state good
  while(n_transferred < n_bytes and fd_state > 0) {
    // Read the remainder to the buff
    fd_state = op(fd, buff+n_transferred, n_bytes-n_transferred);
    // If successful update bytes read
    if (fd_state > 0)
      n_transferred += fd_state;
  }
  // Return fd state on failure or number of bytes read
  return fd_state < 0 ? fd_state : n_transferred;
}

inline int
read_available(Transfer& t)
{
  return transfer_available(t, ::read);
}

inline int
read_all(Transfer& t)
{
  return transfer_all(t, ::read);
}

inline int
write_available(Transfer& t)
{
  return transfer_available(t, ::write);
}

inline int
write_all(Transfer& t)
{
  return transfer_all(t, ::write);
}


} // namespace freeflow
