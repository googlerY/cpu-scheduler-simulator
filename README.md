# 커스텀 CPU 스케줄러 시뮬레이터 (Custom CPU Scheduler Simulator)

## 📌 프로젝트 개요
본 프로젝트는 C언어로 구현된 커스텀 CPU 스케줄러 시뮬레이터로, **실시간(Real-Time) 처리**와 **동적 우선순위 패널티(에이징, Aging)**에 중점을 두고 프로세스 스케줄링 성능을 평가하기 위해 설계되었습니다.

총 5개의 큐로 구성된 **다중 큐(Multi-Level Queue)** 아키텍처를 사용합니다. 실시간 프로세스의 비선점형 실행을 보장하여 중요 작업을 최우선으로 처리합니다. 동시에, 타임 퀀텀 내에 작업을 끝내지 못한 프로세스에는 패널티를 부여해 하위 큐로 강등(Demotion)시킴으로써, 긴 작업의 CPU 독점을 막고 실행 시간이 짧은 작업의 응답성을 극대화하는 피드백 구조를 채택했습니다.

## ⚙️ 스케줄링 아키텍처

시스템은 프로세스의 초기 우선순위 값에 따라 총 5개의 큐(Q0 ~ Q4)로 작업을 분류합니다:

*   **Q0 (우선순위 -10 ~ -1) : 실시간 큐 (Real-Time Queue)**
    *   **정책:** 비선점형 (Non-preemptive)
    *   **동작:** Q0에 배정된 프로세스가 CPU를 점유하면 타임 퀀텀의 제약을 받지 않고 자신의 전체 `computing_time` 동안 실행을 100% 보장받습니다. 이를 통해 엄격한 실시간 데드라인을 충족시킵니다.
*   **Q1 ~ Q3 (우선순위 1 ~ 30) : 패널티 기반 우선순위 큐**
    *   **정책:** 선점형 (타임 퀀텀 기반)
    *   **동작:** 할당된 타임 퀀텀 동안 실행을 보장받습니다. 타임 퀀텀 내에 프로세스가 종료되지 않으면 **패널티(우선순위 값 +10)**를 받아 더 낮은 등급의 큐로 강등되며, 다른 작업에 CPU를 양보합니다. (우선순위는 최대 31로 제한됩니다).
*   **Q4 (우선순위 31) : 라운드 로빈 큐 (Round-Robin Queue)**
    *   **정책:** 라운드 로빈 (RR)
    *   **동작:** 표준 RR 스케줄링 방식을 따릅니다. 타임 퀀텀을 초과하더라도 추가 패널티 없이 Q4의 맨 뒤에 다시 삽입됩니다.

## 🛠️ 핵심 자료 구조
*   **프로세스 제어 블록 (PCB):** 프로세스의 핵심 메타데이터(`pid`, `priority`, `computing_time`, `remaining_time`, `arrival_time`, `end_time`)를 관리하는 구조체입니다.
*   **이중 연결 리스트 큐 (Doubly Linked List Queue):** 표준 FIFO 방식의 큐 연산뿐만 아니라, Q1~Q3를 위한 최적화된 우선순위 기반 삽입(`enqueue_by_priority`)을 지원하기 위해 직접 구현한 자료 구조입니다.

## 📊 성능 분석 결과
표준 테스트 데이터 셋을 이용한 시뮬레이션 결과는 다음과 같습니다:
*   본 스케줄러는 실시간 큐(Q0)에 할당된 중요 작업들이 타임 퀀텀 만료로 인한 잦은 문맥 교환(Context Switching) 오버헤드로부터 완전히 독립되어, CPU를 빼앗기지 않고 즉각적이고 지속적으로 실행됨을 성공적으로 보장했습니다.
*   정규화된 반환 시간(Normalized Turnaround Time, NTT) 분석 결과, 이 아키텍처는 일반적인 혼합 알고리즘 스케줄러와 비교했을 때 **'우선순위가 높은 프로세스의 엄격한 실시간 실행'**이 최우선 목표인 시스템 환경에서 압도적으로 유리함을 입증했습니다.

## 🚀 빌드 및 실행 방법

```bash
# 저장소 클론 (Clone the repository)
git clone [https://github.com/googlerY/cpu-scheduler-simulator.git](https://github.com/googlerY/cpu-scheduler-simulator.git)
cd cpu-scheduler-simulator

# 첫 번째 알고리즘 빌드 (Build the First Algorithm - Real-Time Scheduler)
make algo1

# 시뮬레이터 실행 (Run the simulator)
./scheduler_realtime.out