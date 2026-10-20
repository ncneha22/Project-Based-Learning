/*
 * bankers_test.js - tests for Week4/js/bankers.js
 * Run from the Week4 folder with:  node tests/bankers_test.js
 */

global.window = global;
require("../js/bankers.js");
const Bankers = window.DeadlockSim.Bankers;

let passed = 0;
let failed = 0;

function check(name, actual, expected) {
    const a = JSON.stringify(actual);
    const e = JSON.stringify(expected);

    if (a === e) {
        passed++;
        console.log("PASS  " + name);
    } else {
        failed++;
        console.log("FAIL  " + name + "\n      expected " + e + "\n      got      " + a);
    }
}

// Textbook example: 5 processes, 3 resource types (totals 10, 5, 7)
const allocation = [[0, 1, 0], [2, 0, 0], [3, 0, 2], [2, 1, 1], [0, 0, 2]];
const maximum = [[7, 5, 3], [3, 2, 2], [9, 0, 2], [2, 2, 2], [4, 3, 3]];
const available = [3, 3, 2];
const state = { available: available, allocation: allocation, maximum: maximum };

// Need matrix
check("Need = Max - Allocation",
    Bankers.calculateNeed(maximum, allocation).need,
    [[7, 4, 3], [1, 2, 2], [6, 0, 0], [0, 1, 1], [4, 3, 1]]);

// 1. Safe state with a valid safe sequence
const safe = Bankers.checkSafety(available, allocation, maximum);
check("1. Safe state", [safe.status, safe.safeSequence],
    ["SAFE", ["P1", "P3", "P4", "P0", "P2"]]);

// 2. Unsafe state (same matrices, Available = [1, 0, 0])
const unsafe = Bankers.checkSafety([1, 0, 0], allocation, maximum);
check("2. Unsafe state", unsafe.status, "UNSAFE");

// 3. Request greater than Need (P1 needs only 1 of R0)
const r3 = Bankers.requestResources(state, 1, [2, 0, 0]);
check("3. Request > Need", [r3.granted, r3.status], [false, "INVALID"]);

// 4. Request greater than Available (P0 asks 4 of R0, only 3 available)
const r4 = Bankers.requestResources(state, 0, [4, 0, 0]);
check("4. Request > Available", [r4.granted, r4.status], [false, "WAIT"]);

// 6. Safely granted request (P1 asks [1, 0, 2])
const r6 = Bankers.requestResources(state, 1, [1, 0, 2]);
check("6. Request granted", [r6.granted, r6.newState.available, r6.newState.allocation[1]],
    [true, [2, 3, 0], [3, 0, 2]]);

// 5. Passes checks 1 and 2 but makes the state unsafe.
// Start from the state after test 6 (Available = [2, 3, 0]); P0 asks [0, 2, 0].
const afterGrant = { available: r6.newState.available,
    allocation: r6.newState.allocation, maximum: maximum };
const r5 = Bankers.requestResources(afterGrant, 0, [0, 2, 0]);
check("5. Request makes state unsafe", [r5.granted, r5.status], [false, "UNSAFE"]);
check("   Rejected request leaves Available unchanged", afterGrant.available, [2, 3, 0]);

// Rollback: the original data must be unchanged after all requests
check("   Original state unchanged", [available, allocation[1], allocation[0]],
    [[3, 3, 2], [2, 0, 0], [0, 1, 0]]);

// 7. Invalid matrix dimensions
const r7 = Bankers.checkSafety([3, 3], allocation, maximum);
check("7a. Available has wrong length", r7.status, "INVALID");
const r7b = Bankers.checkSafety(available, allocation, [[7, 5, 3]]);
check("7b. Maximum has wrong number of rows", r7b.status, "INVALID");
const r7c = Bankers.requestResources(state, 1, [1, 0]);
check("7c. Request has wrong length", r7c.status, "INVALID");
const r7d = Bankers.requestResources(state, 9, [0, 0, 0]);
check("7d. Invalid process index", r7d.status, "INVALID");

// 8. Negative and non-integer values, and Allocation > Maximum
check("8a. Negative Available", Bankers.checkSafety([-1, 3, 2], allocation, maximum).status, "INVALID");
check("8b. Non-integer Allocation",
    Bankers.checkSafety(available, [[0, 1.5, 0], [2, 0, 0], [3, 0, 2], [2, 1, 1], [0, 0, 2]], maximum).status,
    "INVALID");
check("8c. Negative request", Bankers.requestResources(state, 1, [-1, 0, 0]).status, "INVALID");
check("8d. Allocation > Maximum",
    Bankers.checkSafety(available, [[8, 1, 0], [2, 0, 0], [3, 0, 2], [2, 1, 1], [0, 0, 2]], maximum).status,
    "INVALID");

// 9. Multiple possible safe sequences
// Both P0 and P1 need [1, 1] and Available is [1, 1], so either can go first.
// The algorithm scans P0 first, so it reports P0 → P1; P1 → P0 is also valid.
const multi = Bankers.checkSafety([1, 1], [[1, 0], [0, 1]], [[2, 1], [1, 2]]);
check("9. Multiple safe sequences (reports one)", [multi.status, multi.safeSequence],
    ["SAFE", ["P0", "P1"]]);

console.log("\n" + passed + " passed, " + failed + " failed");
