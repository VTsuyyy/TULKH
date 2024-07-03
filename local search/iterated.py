import random
import sys

def read_input():
    input = sys.stdin.read
    data = input().split()
    
    index = 0
    N = int(data[index])
    M = int(data[index + 1])
    index += 2
    
    class_info = []
    for i in range(N):
        t = int(data[index])
        g = int(data[index + 1])
        s = int(data[index + 2])
        class_info.append((t, g, s))
        index += 3

    room_capacities = []
    for i in range(M):
        room_capacities.append(int(data[index]))
        index += 1
    
    return N, M, class_info, room_capacities

def evaluate_schedule(schedule, class_info, room_capacities):
    total_classes = len(schedule)
    conflicts = 0
    assigned_classes = set()
    teacher_schedule = {}
    room_schedule = {}

    for cls in schedule:
        cls_id, start_slot, room = cls
        t, g, s = class_info[cls_id - 1]
        
        valid = True

        for slot in range(start_slot, start_slot + t):
            # Kiểm tra nếu vượt quá số slot có sẵn
            if slot > 60:
                valid = False
                break

            # Kiểm tra sức chứa phòng học
            if room_capacities[room - 1] < s:
                valid = False
                break

            # Kiểm tra lịch của giáo viên
            if (g, slot) in teacher_schedule:
                valid = False
                break

            # Kiểm tra lịch phòng học
            if (room, slot) in room_schedule:
                valid = False
                break

        if valid:
            for slot in range(start_slot, start_slot + t):
                teacher_schedule[(g, slot)] = cls_id
                room_schedule[(room, slot)] = cls_id
            assigned_classes.add(cls_id)

    return len(assigned_classes), assigned_classes


def generate_neighbors(schedule, class_info, room_capacities):
    neighbors = []
    for i in range(10):
        neighbor = schedule[:]
        for _ in range(3):
            i = random.randint(0, len(neighbor) - 1)
            cls_id, _, _ = neighbor[i]
            
            t, _, _ = class_info[cls_id - 1]
            
            valid = False
            while not valid:
                new_slot = random.randint(1, 60-t+1)
                new_room = random.randint(1, len(room_capacities))
                
                valid = True
                for slot in range(new_slot, new_slot + t):
                    if slot > 60:
                        valid = False
                        break
                    for _, slot_used, room_used in neighbor:
                        if slot == slot_used and room_used == new_room:
                            valid = False
                            break
                    if not valid:
                        break
            
            neighbor[i] = (cls_id, new_slot, new_room)
        neighbors.append(neighbor)
    return neighbors

def initial_solution_greedy(N, M, class_info, room_capacities):
    schedule = []
    teacher_schedule = {}
    room_schedule = {}
    
    sorted_classes = sorted(range(N), key=lambda x: class_info[x][2], reverse=True)
    
    for cls_id in sorted_classes:
        t, g, s = class_info[cls_id]
        
        best_slot = None
        best_room = None
        min_conflicts = float('inf')
        
        for slot in range(1, 60 - t + 2):
            for room in range(1, M + 1):
                conflicts = 0
                if room_capacities[room - 1] < s:
                    conflicts += 1
                for tslot in range(slot, slot + t):
                    if (g, tslot) in teacher_schedule or (room, tslot) in room_schedule:
                        conflicts += 1
                if conflicts < min_conflicts:
                    min_conflicts = conflicts
                    best_slot = slot
                    best_room = room
        
        schedule.append((cls_id + 1, best_slot, best_room))
        
        for tslot in range(best_slot, best_slot + t):
            teacher_schedule[(g, tslot)] = cls_id + 1
            room_schedule[(best_room, tslot)] = cls_id + 1
    
    return schedule

def local_search(N, M, class_info, room_capacities, max_iterations=600):
    current_schedule = [(i + 1, random.randint(1, 60), random.randint(1, M)) for i in range(N)]
    best_value, best_assigned = evaluate_schedule(current_schedule, class_info, room_capacities)

    for _ in range(max_iterations):
        neighbors = generate_neighbors(current_schedule, class_info, room_capacities)
        for neighbor in neighbors:
            value, assigned = evaluate_schedule(neighbor, class_info, room_capacities)
            
            if value > best_value:
                current_schedule = neighbor
                best_value = value
                best_assigned = assigned

    return best_value, best_assigned, current_schedule

N, M, class_info, room_capacities = read_input()

best_value, best_assigned, final_schedule = None, None, None
for i in range(20):
    current_value, current_assigned, current_schedule = local_search(N, M, class_info, room_capacities)
    if best_value is None or current_value > best_value:
        best_value, best_assigned, final_schedule = current_value, current_assigned, current_schedule

print(len(best_assigned))
for cls in final_schedule:
    cls_id, slot, room = cls
    if cls_id in best_assigned:
        print(cls_id, slot, room)
