# adjust urself 
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

mkdir -p .build

# Build the server
echo "Building the client..."
g++ src/server.cpp -o .build/client -lenet

# Build the server
echo "Building the server..."
g++ src/server.cpp -o .build/server -lenet

echo "Build complete."
