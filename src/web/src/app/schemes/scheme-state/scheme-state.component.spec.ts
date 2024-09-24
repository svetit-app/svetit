import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { SchemeStateComponent } from './scheme-state.component';

describe('SchemeStateListComponent', () => {
  let component: SchemeStateComponent;
  let fixture: ComponentFixture<SchemeStateComponent>;

  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
    imports: [SchemeStateComponent]
})
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(SchemeStateComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
