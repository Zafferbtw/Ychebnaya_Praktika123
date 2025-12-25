import SwiftUI

struct ContentView: View {
    @State private var aText: String = "1 2 3\n4 5 6\n7 8 9"
    @State private var bText: String = "9 8 7\n6 5 4\n3 2 1"
    @State private var result: String = ""

    func parseMatrix(_ text: String) throws -> [[Double]] {
        let rows = text
            .split(separator: "\n")
            .map { row in
                row
                    .split(separator: " ")
                    .compactMap { Double($0) }
            }

        guard !rows.isEmpty else {
            throw MatrixError.empty
        }

        let cols = rows[0].count
        guard cols > 0 else {
            throw MatrixError.invalid
        }

        for r in rows {
            if r.count != cols {
                throw MatrixError.notRectangular
            }
        }

        return rows
    }

    func multiply() {
        do {
            let A = try parseMatrix(aText)
            let B = try parseMatrix(bText)

            let n = A.count
            guard A.allSatisfy({ $0.count == n }) else {
                throw MatrixError.notSquare
            }
            guard B.count == n && B.allSatisfy({ $0.count == n }) else {
                throw MatrixError.sizeMismatch
            }

            var C = Array(repeating: Array(repeating: 0.0, count: n), count: n)

            for i in 0..<n {
                for k in 0..<n {
                    for j in 0..<n {
                        C[i][j] += A[i][k] * B[k][j]
                    }
                }
            }

            result = C
                .map { row in
                    row.map { String(format: "%.2f", $0) }.joined(separator: " ")
                }
                .joined(separator: "\n")

        } catch {
            result = "error: \(error.localizedDescription)"
        }
    }

    var body: some View {
        VStack(alignment: .leading, spacing: 12) {
            Text("matrix calculator")
                .font(.title)

            Text("matrix A")
            TextEditor(text: $aText)
                .frame(height: 80)
                .border(Color.gray)

            Text("matrix B")
            TextEditor(text: $bText)
                .frame(height: 80)
                .border(Color.gray)

            Button("multiply", action: multiply)
                .buttonStyle(.borderedProminent)

            Text("result")
            ScrollView {
                Text(result)
                    .font(.system(.body, design: .monospaced))
            }

            Spacer()
        }
        .padding()
    }
}

enum MatrixError: LocalizedError {
    case empty
    case invalid
    case notRectangular
    case notSquare
    case sizeMismatch

    var errorDescription: String? {
        switch self {
        case .empty:
            return "matrix is empty"
        case .invalid:
            return "invalid matrix"
        case .notRectangular:
            return "rows have different lengths"
        case .notSquare:
            return "matrix must be square"
        case .sizeMismatch:
            return "matrix sizes do not match"
        }
    }
}
