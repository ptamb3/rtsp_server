# RTSP Server

A lightweight **RTSP (Real Time Streaming Protocol) server** written in **C** using **GStreamer** and **libgstrtspserver**.  
This project demonstrates how to set up a simple video streaming service that clients like VLC or FFplay can connect for redering the video.
**Note** The pipeline currently supports video file only but can easily be updated to add audio plugin elements.

---

## 📦 Features

- Simple RTSP server built using the GStreamer RTSP API  
- Supports local MP4 or camera stream sources  
- Lightweight and portable — builds cleanly with CMake or Makefile  
- Out-of-tree build support to keep your source tree clean  
- Ready for integration into embedded or edge video systems  

---

## 🧰 Prerequisites

Ensure you have the required development tools and GStreamer libraries installed.

The repository includes a helper script for this:

```bash
sudo ./install_dependencies.sh
```

This script installs:
- `cmake`, `pkg-config`, `vim`
- `libgstreamer1.0-dev`
- `libgstrtspserver-1.0-dev`
- `gstreamer1.0-plugins-bad`, `gstreamer1.0-plugins-ugly`
- `gstreamer1.0-libav`
- `libx264-dev`

> 🛈 The script checks for each dependency and installs it only if missing.

---

## 🏗️ Building the Project

You can build the RTSP server using **CMake (recommended)** or a **Makefile (simple alternative)**.

---

### 🧱 Option 1: Out-of-Tree Build with CMake (Recommended)

This is the cleanest approach since it keeps build artifacts separate from your source files.

#### **1. Create a Build Directory**
```bash
mkdir -p build
cd build
```

#### **2. Run CMake**
Point CMake to your project source (assuming it’s the parent directory):
```bash
cmake ..
```

If your `CMakeLists.txt` is located elsewhere, replace `..` with the correct path.

#### **3. Build the Binary**
```bash
make
```

After the build completes successfully, you’ll find:
```
build/rtsp_server
```

#### **4. (Optional) Install System-Wide**
```bash
sudo make install
```
By default, this installs to `/usr/local/bin/rtsp_server`.

---

### ⚙️ Optional CMake Options

#### **Set Build Type**
You can build in `Release` or `Debug` mode:
```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
```

#### **Parallel Build**
Speed up compilation using all CPU cores:
```bash
make -j$(nproc)
```

#### **Change Binary Output Directory**
```bash
cmake -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=../bin ..
```

#### **Change Install Prefix**
```bash
cmake -DCMAKE_INSTALL_PREFIX=/opt/rtsp ..
```

---

### 🧰 Option 2: Build Using Makefile

For a simple build using the provided `Makefile`, run:

```bash
make
```

This generates the `rtsp_server` binary in the same directory.

To clean up:
```bash
make clean
```

---

## 🚀 Running the RTSP Server

Once built, you can start the RTSP server directly:

```bash
./rtsp_server
```

The server will start listening (default port `8554`).

You can test the stream using:
```bash
ffplay rtsp://127.0.0.1:8554/test
```

or using VLC:
```
Media → Open Network Stream → rtsp://127.0.0.1:8554/test
```

---

## 🧼 Cleaning the Build

If you want a completely fresh build:

```bash
cd build
rm -rf *
cmake ..
make
```

---

## 🩻 Troubleshooting

### **CMake Errors**
- If you see:
  ```
  Could NOT find PkgConfig
  ```
  install it:
  ```bash
  sudo apt-get install pkg-config
  ```

### **Missing Dependencies**
If GStreamer libraries are missing, rerun:
```bash
sudo ./install_dependencies.sh
```

### **Port Already in Use**
If port `8554` is occupied:
```bash
sudo lsof -i :8554
sudo kill <PID>
```
Then restart your RTSP server.

---

## 📁 Project Structure

```
rtsp_server/
├── CMakeLists.txt
├── Makefile
├── rtsp_server.c
├── install_dependencies.sh
├── LICENSE
└── README.md
```

---

## 💡 Example Build and Run Summary

```bash
# Step 1: Install dependencies
sudo ./install_dependencies.sh

# Step 2: Create and enter build directory
mkdir -p build && cd build

# Step 3: Configure with CMake
cmake ..

# Step 4: Build
make -j$(nproc)

# Step 5: Run
./rtsp_server
```

---

## 🪪 License

This project is distributed under the terms of the [MIT LICENSE](LICENSE) file.  
You are free to use, modify, and distribute it as permitted by the license.

---

## 👤 Author

Maintained by **Pratik Tambe**  
For issues or suggestions, please open a GitHub issue.
