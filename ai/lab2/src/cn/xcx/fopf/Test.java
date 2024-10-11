package cn.xcx.fopf;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.serializer.SerializerFeature;

import cn.xcx.fopf.exception.ParseException;

public class Test {
	public static void main(String[] args) throws ParseException {
		String s = "(∃ x1 )(¬pq (aa , b0, f ( ce9, dd1))∨ ((∀x)p( a) ∧ t(c, g(k)) → q(b ) ∧r (c,d) ↔ s(z)))";
		Formula f = Formula.parse(s);
		System.out.println(JSON.toJSONString(f));  // 序列化
		
		String json = JSON.toJSONString(f, SerializerFeature.WriteClassName);
		Formula df = (Formula) JSON.parse(json);  // 反序列化
		
		System.out.println(JSON.toJSONString(df)); 
	}
}
