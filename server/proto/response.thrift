// 0 or 1.
struct InstantSearchInfo {
  1: required i32 status,
}

struct RunningInfo {
  1: required i32 status,
  2: required i32 total_cost,
  3: required i32 search_cost,
  4: optional list<string> debug_info,
  5: optional list<string> info,
  6: optional list<string> err_info,
  7: optional list<string> score_infos,
  8: optional InstantSearchInfo instant_info,
}

struct Response {
  // the raw_id in doc info
  1: required list<i32> result_docids,
  2: optional list<i32> doc_ids,
  3: required RunningInfo running_info,
}
