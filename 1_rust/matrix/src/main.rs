use std::io;

fn main() {
    let mut input = String::new();
    
    // 크기 입력 
    println!("배열의 크기 n, m 입력 : ");
    io::stdin().read_line(&mut input).expect("읽기 실패");
    let sizes: Vec<usize> = input
        .trim()
        .split_whitespace()
        .map(|s| s.parse().expect("숫자를 입력해라"))
        .collect();

    let n = sizes[0];
    let m = sizes[1];

    // 배열 생성
    let mut matrix1 = vec![vec![0; m]; n];
    let mut matrix2 = vec![vec![0; m]; n];
    let mut matrix3 = vec![vec![0; m]; n];

    // 값 입력 및 계산 
    println!("첫 번째 배열의 값 입력: ");
    for i in 0..n{
        input.clear();
        io::stdin().read_line(&mut input).expect("읽기 실패");
        let values: Vec<i32> = input
            .trim()
            .split_whitespace()
            .map(|s| s.parse().expect("숫자를 입력해라"))
            .collect();
        for j in 0..m {
            matrix1[i][j] = values[j];
        }
    }

    println!("두 번째 배열의 값 입력: ");
    for i in 0..n{
        input.clear(); // 이전 입력 지우기
        io::stdin().read_line(&mut input).expect("읽기 실패");
        let values: Vec<i32> = input
            .trim()
            .split_whitespace()
            .map(|s| s.parse().expect("숫자를 입력해라"))
            .collect();
        for j in 0..m {
            matrix2[i][j] = values[j];
        }
    }

    for i in 0..n{
        for j in 0..m{
            matrix3[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }

    // 결과 출력
    println!("결과: ");
    for mat in matrix3{
        for m in mat{
            print!("{} ", m);
        }
        println!();
    }
    
}
