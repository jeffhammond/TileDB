.. _dense-arrays:

Dense Arrays
============

In this tutorial we will learn how to create, read, and write a simple dense
array in TileDB.

**Link to full programs**

* `C++ <{tiledb_src_root_url}/examples/cpp_api/quickstart_dense.cc>`__
* `Python <{tiledb_py_src_root_url}/examples/quickstart_dense.py>`__

Basic concepts and definitions
------------------------------

.. toggle-header::
    :header: **Cell**, **Dimension**, **Domain**

      An array in TileDB is an n-dimensional collection of cells, where each cell
      is uniquely identified by a coordinate tuple equal to the dimensionality of
      the array. For example, every cell in a 2D array is represented by a coordinate
      pair ``(i, j)``, whereas in a 3D array by a coordinate triple, ``(i, j, k)``.
      Each dimension in the array has an associated domain which
      defines the data type and extent ``(min, max)`` of coordinate values for that
      dimension. The dimension domain could be of type ``int8``, ``uint8``,
      ``int16``, ``uint16``, ``int32``, ``uint32``, ``int64``, ``uint64``,
      ``float32``, or ``float64``. Notice that TileDB supports negative as well as
      real dimensions domains, but for now we will only focus on positive integer
      domains. The ordered set of dimensions comprise the array domain.

      .. note::

        In TileDB, currently all dimension domains must have the same type.

.. toggle-header::
    :header: **Attribute**

      In TileDB, a cell is not limited to storing a single value. Each
      cell stores a tuple with a structure that is common to
      all cells. Each tuple element corresponds to a value on a named attribute
      of a certain type. The array cells can be perceived as rows in a table,
      where each column is an attribute and each row is uniquely identified by
      the cell coordinates. An attribute can specify a single value of type
      ``char``, ``int8``, ``uint8``, ``int16``, ``uint16``, ``int32``, ``uint32``,
      ``int64``, ``uint64``, ``float32``, or ``float64``, or a fixed- or
      variable-sized vector of the above primitive types.

.. toggle-header::
    :header: **Array schema**

      The structure of the array, i.e., the number of dimensions and type of their
      domains, the number and type of attributes (and a lot of other information
      covered in later tutorials) are all defined in the array schema. The array
      schema is very similar to a table schema used in Databases.


.. toggle-header::
    :header: **Dense array**

      If every cell in the array has an associated value, such as a pixel in a 2D image,
      we call the array dense.

.. toggle-header::
    :header: **Array directory**

      An array is stored on persistent storage as a directory containing subdirectories
      and files. We will explain in later tutorials the benefits from such a physical
      organization, and how a "directory" translates for storage backends where
      directories are not treated in the same manner as in a local POSIX filesystem
      (e.g., for the S3 object store).

.. toggle-header::
    :header: **Subarray**

      A subarray is a slice of the array domain, used in queries.


Creating a dense array
----------------------

The following snippet configures the array schema for this tutorial. An array
schema configures parameters such as the number and type of dimensions,
attributes, etc.

.. content-tabs::

   .. tab-container:: cpp
      :title: C++

      .. code-block:: c++

         Context ctx;
         ArraySchema schema(ctx, TILEDB_DENSE);

         // The array will be 4x4 with dimensions "rows" and "cols", with domain [1,4].
         Domain domain(ctx);
         domain.add_dimension(Dimension::create<int>(ctx, "rows", {{1, 4}}, 4))
           .add_dimension(Dimension::create<int>(ctx, "cols", {{1, 4}}, 4));
         
         // The array will be dense.
         ArraySchema schema(ctx, TILEDB_DENSE);
         schema.set_domain(domain).set_order({{TILEDB_ROW_MAJOR, TILEDB_ROW_MAJOR}});
         
         // Add a single attribute "a" so each (i,j) cell can store an integer.
         schema.add_attribute(Attribute::create<int>(ctx, "a"));

   .. tab-container:: python
      :title: Python

      .. code-block:: python

         ctx = tiledb.Ctx()
         
         # The array will be 4x4 with dimensions "rows" and "cols", with domain [1,4].
         dom = tiledb.Domain(ctx,
                             tiledb.Dim(ctx, name="rows", domain=(1, 4), tile=4, dtype=np.int32),
                             tiledb.Dim(ctx, name="cols", domain=(1, 4), tile=4, dtype=np.int32))
         
         # The array will be dense with a single attribute "a" so each (i,j) cell can store an integer.
         schema = tiledb.ArraySchema(ctx, domain=dom, sparse=False,
                                     attrs=[tiledb.Attr(ctx, name="a", dtype=np.int32)])

The array has a 2D domain where the coordinates can be integer values
from 1 to 4 (inclusive) along both dimensions. For now, you can ignore
the tile extent argument used when creating the dimension objects.

.. note::

   The order of the dimensions (as added to the domain) is important later when
   specifying subarrays. For instance, in the above example, subarray
   ``[1,2], [2,4]`` means slice the first two values in the ``rows`` dimension
   domain, and values ``2,3,4`` in the ``cols`` dimension domain.

The array has a single attribute named ``a`` that will hold a single integer
for each cell.

All that is left to do is create the empty array on disk so that it can be written to.
We specify the name of the array to create, and the schema to use. This command
will essentially persist the array schema we just created on disk.

.. content-tabs::

   .. tab-container:: cpp
      :title: C++

      .. code-block:: c++

        std::string array_name("quickstart_dense");
        Array::create(array_name, schema);

   .. tab-container:: python
      :title: Python

      .. code-block:: python

        array_name = "quickstart_dense"
        tiledb.DenseArray.create(array_name, schema)

.. note::

  The array name here can actually be a full URI, for example a path like
  ``file:///home/username/my_array`` or an S3 URI like
  ``s3://bucket-name/array-name``.


Writing to the array
--------------------

We will populate the array with values ``1, 2, ..., 16``.
To start, prepare the data to be written:

.. content-tabs::

   .. tab-container:: cpp
      :title: C++

      .. code-block:: c++

        std::vector<int> data = {
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

   .. tab-container:: python
      :title: Python

      .. code-block:: python

        # Remember to 'import numpy as np'.
        data = np.array(([1, 2, 3, 4],
                         [5, 6, 7, 8],
                         [9, 10, 11, 12],
                         [13, 14, 15, 16]))

Next, open the array for writing and write to the array:

.. content-tabs::

   .. tab-container:: cpp
      :title: C++

      .. code-block:: c++

        Context ctx;
        Array array(ctx, array_name, TILEDB_WRITE);

        // Prepare the query
        Query query(ctx, array);
        query.set_layout(TILEDB_ROW_MAJOR).set_buffer("a", data);

        // Submit the query and close the array.
        query.submit();
        array.close();

   .. tab-container:: python
      :title: Python

      .. code-block:: python

        ctx = tiledb.Ctx()
        with tiledb.DenseArray(ctx, array_name, mode='w') as A:
            A[:] = data

In C++ we use a ``Query`` object, set the buffer for attribute ``a``, and also set the
layout of the cells in the buffer to row-major. Although the cell layout is
covered thoroughly in later tutorials, here what you should know is that
you are telling TileDB that the cell values in your buffer will be written
in row-major order in the cells of the array (i.e., ``1`` will be stored
in cell ``(1,1)``, ``2`` in ``(1,2)``, etc.).

The array data is now stored on disk.
The resulting array is depicted in the figure below.

.. figure:: ../figures/quickstart_dense.png
   :align: center
   :scale: 40 %

Reading from the array
----------------------

We will next explain how to read the cell values in subarray
``[1,2], [2,4]``, i.e., in the blue rectangle shown in the figure above.
The result values should be ``2 3 4 6 7 8``, reading in
row-major order (i.e., first the three selected columns of row ``1``,
then the three selected columns of row ``2``).

Reading happens in much the same way as writing, except in C++ we must provide
buffers sufficient to hold the data being read:L

.. content-tabs::

   .. tab-container:: cpp
      :title: C++

      .. code-block:: c++

        Context ctx;
        Array array(ctx, array_name, TILEDB_READ);
        const std::vector<int> subarray = {1, 2, 2, 4};

        // Prepare the vector that will hold the result (of size 6 elements)
        std::vector<int> data(6);

        // Prepare the query
        Query query(ctx, array, TILEDB_READ);
        query.set_subarray(subarray)
          .set_layout(TILEDB_ROW_MAJOR)
          .set_buffer("a", data);

        // Submit the query and close the array.
        query.submit();
        array.close();

   .. tab-container:: python
      :title: Python

      .. code-block:: python

        ctx = tiledb.Ctx()
        with tiledb.DenseArray(ctx, array_name, mode='r') as A:
            # Slice only rows 1, 2 and cols 2, 3, 4.
            data = A[1:3, 2:5]
            print(data["a"])

Note in C++ the subarray is specified terms of ``(min, max)`` values on each
dimension; in Python we use the usual slicing syntax instead.

In C++ you also must ensure the buffer that will hold the result
has enough space (six elements here, as the result
of the subarray will be six integers). Proper result buffer allocation
is an important topic that is covered in detail in later tutorials.

The row-major layout for the query means that the cells will be returned
in row-major order **within the subarray** ``[1,2], [2,4]`` (more information
on cell layouts is covered in later tutorials).

Now ``data`` holds the result cell values on attribute ``a``.
If you compile and run the example of this tutorial as shown below, you should
see the following output:

.. content-tabs::

   .. tab-container:: cpp
      :title: C++

      .. code-block:: bash

        $ g++ -std=c++11 quickstart_dense.cc -o quickstart_dense -ltiledb
        $ ./quickstart_dense
        2 3 4 6 7 8

   .. tab-container:: python
      :title: Python

      .. code-block:: bash

        $ python quickstart_dense.py
        [[2 3 4]
         [6 7 8]]


On-disk structure
-----------------

A TileDB array is stored on disk as a directory with the name given at the time of array creation.
If we look into the array on disk after it has been written to, we will see something like the following

.. code-block:: bash

   $ ls -l quickstart_dense/
   total 8
   drwx------  4 tyler  staff  136 Jun 11 18:30 __0c4739ed957b4f5eaf0b2738cb1bec1c_1528756214526
   -rwx------  1 tyler  staff  164 Jun 11 18:30 __array_schema.tdb
   -rwx------  1 tyler  staff    0 Jun 11 18:30 __lock.tdb

The array directory and files ``__array_schema.tdb`` and ``__lock.tdb`` were written upon
array creation, whereas subdirectory ``__0c4739ed957b4f5eaf0b2738cb1bec1c_1528756214526`` was
created after array writting. This subdirectory, called **fragment**, contains the written
cell values for attribute ``a`` in file ``a.tdb``, along with associated metadata:

.. code-block:: bash

    $ ls -l quickstart_dense/__0c4739ed957b4f5eaf0b2738cb1bec1c_1528756214526/
    total 16
    -rwx------  1 tyler  staff  117 Jun 11 18:30 __fragment_metadata.tdb
    -rwx------  1 tyler  staff    4 Jun 11 18:30 a.tdb

The TileDB array hierarchy on disk and more details about fragments are discussed in
later tutorials.
