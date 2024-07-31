program workshare_example
! use omp_lib
  implicit none
  integer, parameter :: N = 10
  integer :: i
  real :: array(N), result(N)
  do i = 1, N
    array(i) = real(i)
  end do

  !$omp parallel shared(array, result)
  !$omp workshare
  result = array * array
  !$omp end workshare
  !$omp end parallel

  print *, 'Squared elements of array:'
  do i = 1, N
    print *, result(i)
  end do
end program workshare_example
